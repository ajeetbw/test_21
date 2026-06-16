"""
Data Pipeline Module for Mock SVN Repository.
This module simulates a data extraction, transformation, and loading (ETL) pipeline.
It generates mock sales data, aggregates metrics, and outputs the results.
"""

import json
import csv
import random
import uuid
import logging
from datetime import datetime, timedelta
from typing import List, Dict, Any
from dataclasses import dataclass, asdict

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)

@dataclass
class SaleRecord:
    transaction_id: str
    product_id: str
    category: str
    quantity: int
    price: float
    timestamp: str

class DataPipeline:
    """Handles generation and processing of sales data."""
    
    CATEGORIES = ["Electronics", "Clothing", "Home", "Sports", "Books"]
    
    def __init__(self, num_records: int = 1000):
        self.num_records = num_records
        self.records: List[SaleRecord] = []
        self.aggregated_data: Dict[str, Any] = {}
        
    def generate_data(self) -> None:
        """Generates random mock sales data."""
        logger.info(f"Generating {self.num_records} mock sales records...")
        now = datetime.now()
        
        for _ in range(self.num_records):
            record = SaleRecord(
                transaction_id=str(uuid.uuid4()),
                product_id=f"PROD-{random.randint(100, 999)}",
                category=random.choice(self.CATEGORIES),
                quantity=random.randint(1, 10),
                price=round(random.uniform(10.0, 500.0), 2),
                timestamp=(now - timedelta(days=random.randint(0, 30), 
                                           hours=random.randint(0, 23))).isoformat()
            )
            self.records.append(record)
        logger.info("Data generation complete.")

    def process_data(self) -> None:
        """Processes and aggregates the raw sales data."""
        logger.info("Processing data...")
        if not self.records:
            logger.warning("No records to process.")
            return

        total_revenue = 0.0
        total_items_sold = 0
        category_revenue: Dict[str, float] = {cat: 0.0 for cat in self.CATEGORIES}
        
        for record in self.records:
            revenue = record.quantity * record.price
            total_revenue += revenue
            total_items_sold += record.quantity
            category_revenue[record.category] += revenue
            
        self.aggregated_data = {
            "summary": {
                "total_transactions": len(self.records),
                "total_items_sold": total_items_sold,
                "total_revenue": round(total_revenue, 2),
                "average_transaction_value": round(total_revenue / len(self.records), 2) if self.records else 0
            },
            "category_breakdown": {
                cat: round(rev, 2) for cat, rev in category_revenue.items()
            }
        }
        logger.info("Data processing complete.")

    def export_to_csv(self, filename: str) -> bool:
        """Exports raw records to a CSV file."""
        logger.info(f"Exporting raw data to {filename}...")
        try:
            with open(filename, 'w', newline='', encoding='utf-8') as f:
                if not self.records:
                    return False
                writer = csv.DictWriter(f, fieldnames=asdict(self.records[0]).keys())
                writer.writeheader()
                for record in self.records:
                    writer.writerow(asdict(record))
            logger.info("CSV export successful.")
            return True
        except Exception as e:
            logger.error(f"Failed to export CSV: {e}")
            return False

    def export_to_json(self, filename: str) -> bool:
        """Exports aggregated data to a JSON file."""
        logger.info(f"Exporting aggregated data to {filename}...")
        try:
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(self.aggregated_data, f, indent=4)
            logger.info("JSON export successful.")
            return True
        except Exception as e:
            logger.error(f"Failed to export JSON: {e}")
            return False

def main():
    """Main execution function."""
    logger.info("Starting Data Pipeline run.")
    pipeline = DataPipeline(num_records=5000)
    
    pipeline.generate_data()
    pipeline.process_data()
    
    pipeline.export_to_csv('raw_sales_data.csv')
    pipeline.export_to_json('aggregated_sales_data.json')
    logger.info("Data Pipeline run finished successfully.")

if __name__ == "__main__":
    main()