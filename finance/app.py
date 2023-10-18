import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Global variables
changePw = False


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Top up cash"""
    if request.method == "POST":
        username = db.execute(
            "SELECT username FROM users WHERE id =? ", session["user_id"]
        )[0]["username"]

        deposit = request.form.get("depositamount")
        # Ensure deposit amount was submitted
        if not deposit:
            return apology("Must send at least one USD.", 403)

        # Query database for user info
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure user is correct
        if username != request.form.get("username"):
            return apology("Invalid username.", 403)

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid password.", 403)

        # Update users table with deposit amount
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id =?", deposit, session["user_id"]
        )

        flash(f"User {username} has deposited ${deposit}.")
        return redirect("/")

    return render_template("withdraw.html")


@app.route("/withdraw", methods=["GET", "POST"])
@login_required
def withdraw():
    """Top up cash"""
    if request.method == "POST":
        username = db.execute(
            "SELECT username FROM users WHERE id =? ", session["user_id"]
        )[0]["username"]

        withdraw = request.form.get("withdrawamount")
        # Ensure withdraw amount was submitted
        if not withdraw:
            return apology("Must draw at least one USD.", 403)

        # Query database for user info
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure user is correct
        if username != request.form.get("username"):
            return apology("Invalid username.", 403)

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid password.", 403)

        # Update users table with deposit amount
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id =?", withdraw, session["user_id"]
        )

        flash(f"User {username} has withdrawn ${withdraw}.")
        return redirect("/")

    return render_template("withdraw.html")


@app.route("/user", methods=["GET", "POST"])
@login_required
def user_settings():
    """Ensures user is authorised to change password"""
    users = db.execute("SELECT id FROM users WHERE id =? ", session["user_id"])

    if request.method == "POST":
        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for user info
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid password.", 403)

        # Track that user is authorised to change password
        changePw = True
        return render_template("user.html", users=users, changePw=changePw)

    return render_template("user.html", users=users)


@app.route("/changepw", methods=["POST"])
@login_required
def change_pw():
    """User changes password"""
    users = db.execute("SELECT id FROM users WHERE id =? ", session["user_id"])
    userNewPw = users[0]["id"]

    newPassword = request.form.get("newPassword")
    if not newPassword:
        return apology("Must provide new password", 403)

    # Password validation
    pattern = r"^(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}$"
    match = re.match(pattern, newPassword)

    if not match:
        return apology("Please choose a stronger password.")

    # Ensure new password confirmation was submitted:
    newConfirmation = request.form.get("newConfirmation")
    if not newConfirmation:
        return apology("Reconfirm password.", 400)

    # Ensure new password and confirmation match:
    elif newPassword != newConfirmation:
        return apology("Passwords do not match.", 400)

    # Generate new hash
    newHash = generate_password_hash(newPassword)

    # Update users info
    db.execute("UPDATE users SET hash = ? WHERE id =?", newHash, session["user_id"])

    flash(f"User {userNewPw} has changed password.")
    return redirect("/")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Displays HTML table for user logged in - stock owned, no. of shares,
    stocks = db.execute(
        "SELECT stock, SUM(shares) as total_shares, total FROM transactions WHERE user_id = ? GROUP BY stock HAVING total_shares > 0",
        session["user_id"],
    )
    # Which stock owned, no. of shares, current price, total value

    # Get user's current cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
        "cash"
    ]

    grand_total = cash

    # Iterate over stocks
    for stock in stocks:  # stock is 1 dict object in the list 'stocks'
        quote = lookup(stock["stock"])  # returns a dict of name, symbol and price
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["total_shares"]
        grand_total += stock["value"]  # Stocks' total value plus cash

    return render_template(
        "index.html", stocks=stocks, cash=cash, grand_total=grand_total
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Must include symbol.", 400)

        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Must buy at least one share.", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("Symbol not found.", 400)

        # Look up stock's current price
        price = quote["price"]
        shareNum = int(shares)
        cashRequired = shareNum * price

        # Select how much cash user currently has
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
            "cash"
        ]  # Returns a list of zero or mode dict objects

        # Check if user can afford number of shares at current price
        if cash < cashRequired:
            return apology("Not enough cash.")

        remainCash = cash - cashRequired

        # Update users table
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", remainCash, session["user_id"]
        )

        db.execute(
            """INSERT INTO transactions
                   (user_id, stock, shares, price, total)
                   VALUES (?,?,?,?,?)
                   """,
            session["user_id"],
            symbol,
            shareNum,
            price,
            cashRequired,
        )

        # Flashing system makes it possible to record a message
        # at the end of a request and access it on the
        # next (and only the next) request
        flash(f"Bought {shareNum} shares of {symbol} for {usd(cashRequired)}")
        return redirect("/")

    # User reached route via GET (via clicking or redirect)
    else:
        return render_template("buy.html")


@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions"""
    # Transaction Type, Stock, how many, date and time
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id =? ORDER BY time DESC",
        session["user_id"],
    )

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Embed one or more values from lookup
    if request.method == "POST":
        symbol = request.form.get("symbol")
        # lookup returns dict with name, price and symbol
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol.", 400)
        return render_template("quoted.html", quote=quote)

    # Show HTML form that submits to /quote if user reached route via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted:
        username = request.form.get("username")
        if not username:
            return apology("Missing username.", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username =?", username)

        # Ensure username does not already exists
        if len(rows) != 0:
            return apology("Username already exists.", 400)

        # Ensure password was submitted:
        password = request.form.get("password")

        if not password:
            return apology("Missing password.", 400)

        # Password validation (See: https://www.educative.io/answers/how-to-do-password-validation-in-python-using-regex)
        pattern = r"^(?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,}$"
        match = re.match(pattern, password)

        if not match:
            return apology("Please choose a stronger password.")

        # Ensure password confirmation was submitted:
        elif not request.form.get("confirmation"):
            return apology("Reconfirm password.", 400)

        # Ensure password and confirmation match:
        elif password != request.form.get("confirmation"):
            return apology("Passwords do not match.", 400)

        # Insert user into finance.db
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?,?)",
            username,
            generate_password_hash(password),
        )

        # Query finance.db for newly inserted user
        rows = db.execute("SELECT * FROM users WHERE username =?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        return redirect("/")

    # User reached route via GET (via clicking or redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get user's stocks
    stocks = db.execute(
        "SELECT stock, SUM(shares) as total_shares, total FROM transactions WHERE user_id = ? GROUP BY stock HAVING total_shares > 0",
        session["user_id"],
    )

    # If user submits form
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Must include symbol.", 400)

        elif not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Must buy at least one share.", 400)

        else:
            shareNum = int(shares)

        for stock in stocks:
            if stock["stock"] == symbol:
                if stock["total_shares"] < shareNum:
                    return apology("You don't have enough shares to sell.")
                else:
                    # Find price of stock to sell
                    quote = lookup(symbol)
                    if not quote:
                        return apology("Symbol not found.", 400)
                    price = quote["price"]
                    totalSale = shareNum * price

                    # Update users table
                    db.execute(
                        "UPDATE users SET cash = cash + ? WHERE id =?",
                        totalSale,
                        session["user_id"],
                    )

                    # Record sale
                    db.execute(
                        "INSERT INTO transactions (user_id, stock, shares, price, total, type) VALUES (?,?,?,?,?,?)",
                        session["user_id"],
                        symbol,
                        -shareNum,
                        price,
                        totalSale,
                        "sold",
                    )

                    flash(f"Sold {shareNum} shares of {stock} for {usd(totalSale)}")
                    # Upon completion of sale
                    return redirect("/")

        return apology("You don't own any shares of this stock.")

    else:
        return render_template("sell.html", stocks=stocks)
