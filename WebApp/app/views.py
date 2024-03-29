from flask import Blueprint, render_template, redirect, url_for, abort, request
import datetime
import secret
import os

views = Blueprint(__name__, "views")

currpath = os.path.dirname(__file__)

@views.route("/<key>")
def home(key):
    if key != secret.presharedkey:
        abort(401)
    else:
        f = open("/app/data/results.csv", "r")
        lineData = f.readlines()
        data = []
        for line in lineData:
            data.append(line.split(","))
        f.close()
        return render_template("index.html", data=data)

@views.route("/log/<location>/<humidity>/<temp>")
def log(location, humidity, temp):
    currentDate = datetime.datetime.now()
    f = open("/app/data/results.csv", "a")
    newLine = f"{currentDate.strftime('%Y-%m-%d %H:%M:%S')},{humidity},{temp},{location}\n"
    f.write(newLine)
    f.close()
    return redirect(url_for("views.home", key=secret.presharedkey))

