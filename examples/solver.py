from fasthtml.common import *

import thermal
import materials.fluid.VG46

url = "http://localhost:5001"

app,rt = fast_app()

def SolverForm():
    return Div(
            H1("Solver settings"),
            P("Select type"),
            cls ="solver_settings"
        )

@rt("/")
def get(): return (
    Title('Dive tools'),
    
    SolverForm()
    )

"""

def solver_available(count):
    return Form(
        Div(
            Hidden(id="count", value=count),
            Div(f"Count: {count}", id="count-display"),
            Button("Increment", hx_post="/increment", hx_target="#counter-container", hx_swap="innerHTML")
        ),
        id="counter-container"
    )

@rt('/')
def get():
    return (
        Socials(
            title="Vercel + FastHTML",
            site_name="Vercel",
            description="A demo of Vercel and FastHTML integration",
            image="https://vercel.fyi/fasthtml-og",
            url="https://fasthtml-template.vercel.app",
            twitter_site="@vercel",
        ),
        Container(
            Card(
                Group(
                    P(
                        "FastHTML is a new next-generation web framework for fast, scalable web applications with minimal, compact code. It builds on top of popular foundations like ASGI and HTMX. You can now deploy FastHTML with Vercel CLI or by pushing new changes to your git repository.",
                    ),
                ),
                header=(Titled("FastHTML + Vercel")),
                footer=(
                    P(
                        A(
                            "Deploy your own",
                            href="https://vercel.com/templates/python/fasthtml-python-boilerplate",
                        ),
                        " or ",
                        A("learn more", href="https://docs.fastht.ml/"),
                        "about FastHTML.",
                    )
                ),
            ),
        ),
    )
"""

serve()