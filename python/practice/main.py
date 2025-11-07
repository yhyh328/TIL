
from fastapi import FastAPI, Query
from fastapi.middleware.cors import CORSMiddleware
import uvicorn
import os

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/")
async def read_root():
    return { "Hello": "World"}


@app.get("/order/apple")
async def read_apple(color: str = Query(max_length=5)):
    if color == "red":
        object = "🍎"
    elif color == "green":
        object = "🍏"
    else:
        return "存在しない色を検索しました。"
    return f"{object}が注文されました。"



if __name__ == "__main__":
    port = int(os.environ.get("PORT", 8000))
    uvicorn.run(app, host="0.0.0.0", port=port)

