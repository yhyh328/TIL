from fastapi import FastAPI, Query, Body
from fastapi.middleware.cors import CORSMiddleware
from model import FruitWorld
from db import session

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)

menu_list:list = []

@app.get("/")
async def read_root(): 
    return { "Hello" : "World" }

@app.get("/menu/{food}")
async def read_menu(food: str): 

    # menu_list_tmp:list = []
    # for menu in menu_list:
    #     if menu["food"] == food:
    #         menu_list_tmp.append(menu)
    # if menu_list_tmp:
    #     return menu_list_tmp
    # return "Not registered food"

    menu = session.query(FruitWorld).filter(FruitWorld.food == food).all()
    return menu if menu else "Not registered food"


@app.get("/menus/")
async def read_menus():
    # return menu_list
    return session.query(FruitWorld).all()

@app.post("/menu/")
async def create_menu(food: str = Body(...), img: str = Body(...)):
    # menu_list.append( { "food" : food, "img" : img } )
    cur_model = FruitWorld()
    cur_model.food, cur_model.img = food, img
    session.add(cur_model)
    session.commit()
    return "Register successed"

@app.put("/menu/")
async def update_menu(food: str = Body(...), img: str = Body(...)):
    for menu in menu_list:
        if menu["food"] == food:
            menu["img"] = img
            return "Update successed"
    return "Not registered food"

@app.delete("/menu/{food}")
async def delete_menu(food: str):
    # for menu in menu_list:
    #     if menu["food"] == food:
    #         menu_list.remove(menu)
    #         return "Delete successed"
    # return "Not registered food"

    try:
        data = session.query(FruitWorld).filter(FruitWorld.food == food).first()
        print('123112312', data)
        if data:
            session.query(FruitWorld).filter(FruitWorld.food == food).delete()
            return "Delete successed"
        else:
            return "Not registered food"
    except:
        return "Something went wrong"
    