import datetime


class Book:
    """Represents a book in the library system."""

    def __init__(self, isbn: str, title: str, author: str, copies: int):
        self.isbn = isbn
        self.title = title
        self.author = author
        self.total_copies = copies
        self.available_copies = copies

    def borrow_book(self) -> bool:
        """Decrements available copies if available."""
        if self.available_copies > 0:
            self.available_copies -= 1
            return True
        return False

    def return_book(self) -> bool:
        """Increments available copies back to total limit."""
        if self.available_copies < self.total_copies:
            self.available_copies += 1
            return True
        return False

    def __str__(self) -> str:
        return (
            f"ISBN: {self.isbn} | '{self.title}' by {self.author} "
            f"({self.available_copies}/{self.total_copies} available)"
        )


class Member:
    """Represents a registered library member."""

    def __init__(self, member_id: str, name: str):
        self.member_id = member_id
        self.name = name
        self.borrowed_books = {}  # Format: {isbn: return_by_date}

    def add_book(self, isbn: str, days: int = 14):
        """Adds a borrowed book with a calculated due date."""
        due_date = datetime.date.today() + datetime.timedelta(days=days)
        self.borrowed_books[isbn] = due_date

    def remove_book(self, isbn: str) -> bool:
        """Removes a book from member record upon return."""
        if isbn in self.borrowed_books:
            del self.borrowed_books[isbn]
            return True
        return False

    def __str__(self) -> str:
        books_count = len(self.borrowed_books)
        return f"ID: {self.member_id} | Name: {self.name} | Borrowed Books: {books_count}"


class Library:
    """Manages collection of books and library members."""

    def __init__(self):
        self.books = {}  # Format: {isbn: Book object}
        self.members = {}  # Format: {member_id: Member object}

    def add_new_book(self, isbn: str, title: str, author: str, copies: int) -> str:
        if isbn in self.books:
            self.books[isbn].total_copies += copies
            self.books[isbn].available_copies += copies
            return f"Updated copies for existing book: {title}."

        new_book = Book(isbn, title, author, copies)
        self.books[isbn] = new_book
        return f"Successfully added '{title}' to catalog."

    def register_member(self, member_id: str, name: str) -> str:
        if member_id in self.members:
            return f"Error: Member ID {member_id} already exists."

        self.members[member_id] = Member(member_id, name)
        return f"Successfully registered member: {name}."

    def issue_book(self, member_id: str, isbn: str) -> str:
        if member_id not in self.members:
            return "Error: Member registration not found."
        if isbn not in self.books:
            return "Error: Book not found in library catalog."
        if isbn in self.members[member_id].borrowed_books:
            return "Error: Member already holding a copy of this book."

        book = self.books[isbn]
        if book.borrow_book():
            self.members[member_id].add_book(isbn)
            return f"Book '{book.title}' successfully checked out to {self.members[member_id].name}."
        return f"Error: '{book.title}' is currently out of stock."

    def receive_return(self, member_id: str, isbn: str) -> str:
        if member_id not in self.members:
            return "Error: Invalid member ID."

        member = self.members[member_id]
        if isbn not in member.borrowed_books:
            return f"Error: Record shows {member.name} did not borrow this book."

        member.remove_book(isbn)
        self.books[isbn].return_book()
        return f"Successfully returned '{self.books[isbn].title}' from {member.name}."

    def display_inventory(self):
        print("\n--- Current Library Inventory ---")
        if not self.books:
            print("Catalog is empty.")
        for book in self.books.values():
            print(book)

    def display_members(self):
        print("\n--- Registered Members ---")
        if not self.members:
            print("No members registered.")
        for member in self.members.values():
            print(member)


def run_system_demo():
    """Runs a simulated terminal demonstration of the library system."""
    lib = Library()

    # Seed data injection
    print(lib.add_new_book("978-0141439518", "Pride and Prejudice", "Jane Austen", 2))
    print(lib.add_new_book("978-0451524935", "1984", "George Orwell", 1))
    print(lib.register_member("M001", "Alice Smith"))
    print(lib.register_member("M002", "Bob Jones"))

    # Initial view
    lib.display_inventory()

    # Simulating transaction rules and constraints
    print("\n--- Processing Checkouts ---")
    print(lib.issue_book("M001", "978-0451524935"))  # Successful checkout
    print(lib.issue_book("M002", "978-0451524935"))  # Fails: out of stock
    print(lib.issue_book("M001", "978-0141439518"))  # Successful checkout

    # Inventory review after updates
    lib.display_inventory()
    lib.display_members()

    # Returns simulation
    print("\n--- Processing Returns ---")
    print(lib.receive_return("M001", "978-0451524935"))  # Successful return
    print(lib.issue_book("M002", "978-0451524935"))  # Now succeeds

    # Final validation print
    lib.display_inventory()


if __name__ == "__main__":
    run_system_demo()
