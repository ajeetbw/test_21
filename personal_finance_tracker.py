import datetime


class Transaction:
    """Represents an individual financial transaction."""

    def __init__(self, amount: float, category: str, t_type: str, description: str):
        self.amount = abs(amount)
        self.category = category.strip().title()
        self.type = t_type.strip().lower()  # 'income' or 'expense'
        self.description = description.strip()
        self.date = datetime.date.today()

    def __str__(self) -> str:
        sign = "+" if self.type == "income" else "-"
        return (
            f"[{self.date}] {self.category:<12} | {sign}${self.amount:<7.2f} | "
            f"{self.description}"
        )


class BudgetTracker:
    """Manages transactions, balances, and category budgets."""

    def __init__(self, initial_balance: float = 0.0):
        self.balance = initial_balance
        self.transactions = []
        self.budgets = {}  # Format: {category: budget_limit}

    def add_income(self, amount: float, category: str, description: str) -> str:
        """Adds income to the account balance."""
        if amount <= 0:
            return "Error: Income amount must be greater than zero."

        transaction = Transaction(amount, category, "income", description)
        self.transactions.append(transaction)
        self.balance += transaction.amount
        return f"Success: Added ${transaction.amount:.2f} to {transaction.category}."

    def add_expense(self, amount: float, category: str, description: str) -> str:
        """Adds an expense and checks against the defined category budget."""
        if amount <= 0:
            return "Error: Expense amount must be greater than zero."
        if amount > self.balance:
            return f"Error: Insufficient funds. Current balance is ${self.balance:.2f}."

        transaction = Transaction(amount, category, "expense", description)
        self.transactions.append(transaction)
        self.balance -= transaction.amount

        # Check budget alert
        budget_alert = ""
        category_name = transaction.category
        if category_name in self.budgets:
            total_spent = self.get_category_total(category_name, "expense")
            if total_spent > self.budgets[category_name]:
                budget_alert = (
                    f"\n⚠️  BUDGET ALERT: You exceeded your ${self.budgets[category_name]:.2f} "
                    f"budget for {category_name}! Total spent: ${total_spent:.2f}."
                )

        return f"Success: Logged ${transaction.amount:.2f} for {transaction.category}.{budget_alert}"

    def set_budget(self, category: str, limit: float) -> str:
        """Sets a monthly spending limit for a specific category."""
        if limit < 0:
            return "Error: Budget limit cannot be negative."
        cat_title = category.strip().title()
        self.budgets[cat_title] = limit
        return f"Success: Budget for {cat_title} set to ${limit:.2f}."

    def get_category_total(self, category: str, t_type: str) -> float:
        """Calculates total money for a specific category and transaction type."""
        cat_title = category.strip().title()
        return sum(
            t.amount
            for t in self.transactions
            if t.category == cat_title and t.type == t_type
        )

    def generate_report(self):
        """Prints a comprehensive overview of financial history."""
        print("\n================ FINANCIAL REPORT ================")
        print(f"Current Net Balance: ${self.balance:.2f}")
        print(f"Total Transactions:  {len(self.transactions)}")
        print("--------------------------------------------------")

        if not self.transactions:
            print("No transactions recorded yet.")
            return

        print(f"{'Date':<12} | {'Category':<12} | {'Amount':<8} | Description")
        print("--------------------------------------------------")
        for t in self.transactions:
            print(t)

        print("\n--- Category Summary ---")
        unique_categories = {t.category for t in self.transactions}
        for cat in unique_categories:
            spent = self.get_category_total(cat, "expense")
            earned = self.get_category_total(cat, "income")

            budget_info = ""
            if cat in self.budgets:
                budget_info = f" (Budget: ${self.budgets[cat]:.2f})"

            print(f"- {cat:<12} | Earned: ${earned:<7.2f} | Spent: ${spent:<7.2f}{budget_info}")
        print("==================================================")


def run_finance_simulation():
    """Simulates realistic financial activities and transactions."""
    # Start tracking with an initial deposit
    wallet = BudgetTracker(initial_balance=500.0)
    print(f"Wallet initialized with a base balance.")

    # Configure categorical budgets
    print(wallet.set_budget("Food", 150.0))
    print(wallet.set_budget("Entertainment", 50.0))

    # Log incoming revenue streams
    print("\n--- Processing Revenue ---")
    print(wallet.add_income(1200.0, "Salary", "Bi-weekly paycheck"))
    print(wallet.add_income(45.0, "Freelance", "Logo design gig"))

    # Log day-to-day expenditures
    print("\n--- Processing Expenditures ---")
    print(wallet.add_expense(85.50, "Food", "Weekly grocery run"))
    print(wallet.add_expense(12.99, "Entertainment", "Streaming subscription"))
    print(wallet.add_expense(45.00, "Transport", "Gas station refill"))

    # Force a budget breach condition
    print("\n--- Testing Budget Boundaries ---")
    print(wallet.add_expense(95.00, "Food", "Fine dining dinner"))  # Exceeds food budget

    # Force an insufficient cash failure condition
    print("\n--- Testing Safety Constraints ---")
    print(wallet.add_expense(3000.0, "Luxury", "Premium laptop purchase"))

    # Output detailed operational analytical review
    wallet.generate_report()


if __name__ == "__main__":
    run_finance_simulation()
