import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        name = request.form.get('name')
        day = request.form.get('day')
        month = request.form.get('month')

        if not name or not day or not month:
            return redirect('/')

        try:
            month = int(month)
            day = int(day)
        except ValueError:
            return redirect('/')

        if (day < 1 or day > 31) or (month < 1 or month > 12):
            return redirect('/')

        db.execute('INSERT INTO birthdays (name, day, month) VALUES(?, ?, ?)', name, day, month)
        return redirect('/')
    else:
        birthdays = db.execute('SELECT * FROM birthdays')

        return render_template("index.html", birthdays = birthdays)


