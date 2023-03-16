from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def index():
    f = open("results.csv", "r")
    lineData = f.readlines()
    data = []
    for line in lineData:
        data.append(line.split(","))
    return render_template("index.html", data=data);

if __name__ == '__main__':
    app.run(debug=True)