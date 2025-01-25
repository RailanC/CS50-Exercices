import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    headings = lookup("A").keys()
    userid = session["user_id"]
    data = db.execute("SELECT * FROM wallet WHERE user_id = ?", userid)
    userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
    return render_template("index.html", data=data, headings=headings, userdata=userdata)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    userid = session["user_id"]
    userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
    if request.method == "POST":
        search = request.form.get("search")
        quantity = request.form.get("quantity")
        quantity = float(quantity) if quantity and quantity.replace('.', '', 1).isdigit() else None

        if not userid:
            session.clear()
            return apology("You have been log out")

        if search:
            quote = lookup(search)

            if not quote:
                return render_template("buy.html", warning=f"Symbol: {search} doesn't exist", userdata=userdata)

            headings = quote.keys()
            data = quote

            return render_template("buy.html", headings=headings, data=data, userdata=userdata)
        elif quantity:
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", userid)
            qtd = request.form.get("quantity")
            qtd = float(qtd) if qtd and qtd.replace('.', '', 1).isdigit() else None
            price = float(request.form.get("price"))

            if qtd < 1:
                return apology("Quantity needs to be bigger then 0")

            if user_cash[0]["cash"] == 0:
                return apology("You are broke!")

            total_price = (price * qtd)
            result = round(user_cash[0]["cash"] - total_price)

            if result < 0:
                return render_template("buy.html", warning=f"You can't buy that many shares! you have {user_cash[0]["cash"]:.2f} in cash", userdata=userdata)


            db.execute("UPDATE users SET cash = ? WHERE id = ?", result, userid)
            sym = request.form.get("symbol")

            hasShare = db.execute("SELECT * FROM wallet WHERE symbol = ? AND user_id = ?", sym, userid)

            if len(hasShare) == 0:
                db.execute("INSERT INTO wallet (user_id,symbol,quantity) VALUES (?,?,?)", userid, sym, qtd)
            else:
                db.execute("UPDATE wallet SET quantity = quantity + ? WHERE symbol = ? AND user_id = ?",qtd ,sym, userid)

            current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            db.execute("INSERT INTO transactions (user_id, type, amount, quantity, data, symbol) VALUES (?, ?, ?, ?, ?, ?)", userid, "bought", round(total_price), qtd,current_time, sym)
            userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
            return render_template("buy.html", message=f"You bought {qtd} stocks of {sym}", userdata=userdata)
        return render_template("buy.html", userdata=userdata)
    else:
        return render_template("buy.html", userdata=userdata)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userid = session["user_id"]
    userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
    data = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY data DESC", userid)

    return render_template("history.html", data=data,userdata=userdata)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    session.clear()

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("must provide username", 403)


        elif not password:
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]
        return redirect("/")
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
    userid = session["user_id"]
    userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
    if (request.method == "POST"):
        search = request.form.get("search")

        if not search:
            return apology("must provide a search")

        if not userid:
            session.clear()
            return apology("You have been log out")

        quote = lookup(search)

        if not quote:
            return render_template("quote.html", exist=search,userdata=userdata)

        headings = quote.keys()
        data = quote.values()

        return render_template("quote.html", headings=headings, data=data, userdata=userdata)
    else:
        return render_template("quote.html", userdata=userdata)



@app.route("/register", methods=["GET", "POST"])
def register():
    if(request.method == "POST"):
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)

        password = request.form.get("password")
        if not password:
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 0:
            return apology("username already exists", 403)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    userid = session["user_id"]
    userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
    data = db.execute("SELECT * FROM wallet WHERE user_id = ?", userid)
    if request.method == "POST":
        shares = request.form.get("shares")
        shares = float(shares) if shares and shares.replace('.', '', 1).isdigit() else None
        if shares == None:
            return render_template("sell.html",data=data)
        quantity = int(request.form.get("quantity"))

        symbol = request.form.get("symbol")
        price = lookup(symbol)["price"]


        if shares < 1:
            return render_template("sell.html",data=data, warning=f"You can't sell 0 or less shares",userdata=userdata)

        if quantity - shares < 0:
            return render_template("sell.html",data=data, warning=f"You can't sell that many shares {shares}",userdata=userdata)

        result_qtd = quantity - shares;
        db.execute("UPDATE wallet SET quantity = ? WHERE user_id = ? AND symbol = ?",result_qtd,userid,symbol)

        result_cash = round(shares * price);
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", result_cash, userid)

        data = db.execute("SELECT * FROM wallet WHERE user_id = ?", userid)
        message = db.execute("SELECT cash FROM users WHERE id = ?", userid)
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        db.execute("INSERT INTO transactions (user_id, type, amount, quantity, data, symbol) VALUES (?, ?, ?, ?, ?, ?)", userid, "sold", result_cash, shares,current_time, symbol)
        userdata = db.execute("SELECT * FROM users WHERE id = ?", userid)
        return render_template("sell.html", data=data, message=f"Your cash has been updated to {message[0]["cash"]} $",userdata=userdata)
    else:
        data = db.execute("SELECT * FROM wallet WHERE user_id = ?", userid)
        return render_template("sell.html", data=data,userdata=userdata)

@app.route("/perfil",methods=["GET", "POST"])
@login_required
def perfil():
    """Show Perfil"""
    userid = session["user_id"]
    user = db.execute("SELECT * FROM users WHERE id = ?", userid)
    if request.method == "POST":
        cash = request.form.get("amount")
        cash = float(cash) if cash and cash.replace('.', '', 1).isdigit() else None
        if cash == None:
            return render_template("perfil.html",userdata=user)
        password = request.form.get("password")
        if cash:
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", cash ,userid)
            cash = db.execute("SELECT * FROM users WHERE id = ?", userid)
            return render_template("perfil.html",userdata=user)
        elif password:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(password), userid)
            return render_template("perfil.html",userdata=user)

        return render_template("perfil.html", userdata=user)
    else:
        return render_template("perfil.html", userdata=user)

