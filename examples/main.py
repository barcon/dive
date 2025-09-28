from flask import Flask, request, url_for
from markupsafe import escape

app = Flask(__name__)

@app.route('/')
def solver_type()->str:
    type = request.args.get('type', 'Iterative')
    return f'<h1>{escape(type)}</h1>'

@app.route('/solver/')
def solver()->str:
    type = request.args.get('type', 'Iterative')
    return f'<h1>Solver Settings: {type}</h1>'

@app.route('/solver/<string:iterative>')
def solver_iterative(iterative)->str:
    return f'<h1>{escape(iterative)} Solver Settings</h1>'

app.run(debug = True) 