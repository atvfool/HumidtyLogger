from flask import Flask, render_template
import datetime

app = Flask(__name__)

@app.route("/")
def index():
    f = open("results.csv", "r")
    lineData = f.readlines()
    data = []
    for line in lineData:
        data.append(line.split(","))
    f.close()
    return render_template("index.html", data=data);

@app.route("/log/<location>/<humidity>/<temp>")
def log(location, humidity, temp):
    currentDate = datetime.datetime.now()
    f = open("results.csv", "a")
    newLine = f"{currentDate},{humidity},{location},{location}\n"
    f.write(newLine)
    f.close()
    app.redirect("/", 200)


if __name__ == '__main__':
    app.run(debug=True)