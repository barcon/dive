from fasthtml.common import *

import thermal
import materials.fluid.VG46

url = "http://localhost:5001"

app,rt = fast_app()

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.VG46.Create(1, T_ref, p_ref)

#meshes.CreateCavity(1.0, 1.0, 0.1, 101, 101, 2, False)
thermal.GmshInitialize()
thermal.GmshOpenFile("cavity.msh")

cavity = thermal.GmshGetMeshForPhysicalGroup(1, 1, "cavity")
wall = thermal.GmshGetNodesForPhysicalGroup(cavity, "wall")
hot = thermal.GmshGetNodesForPhysicalGroup(cavity, "hot")
plot = thermal.GmshGetNodesForPhysicalGroup(cavity, "plot")

thermal.GmshFinalize()

thermal.CreateProblem(1, cavity, pressure)
thermal.ApplyMaterial(cavity, material)
thermal.ApplyDirichlet(hot, 100.0)
thermal.ApplyDirichlet(wall, 0.0)
thermal.Initialize()

@rt('/')
def get():
    numberElements = len(cavity.GetElements())
    return Div(P('Number Elements = ' + str(numberElements), hx_get="/change"))

serve()

"""
app, rt = fast_app(hdrs=(picolink))


@rt("/")
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


serve()
"""
