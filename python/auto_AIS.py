from playwright.sync_api import sync_playwright
from dotenv import load_dotenv
import os, time

### CONFIG 
load_dotenv()
AIS_ID, AIS_PWD = os.getenv('AIS_ID'), os.getenv('AIS_PWD')

with sync_playwright() as p:
    browser = p.chromium.launch(headless=False)
    page = browser.new_page()
    page.goto('https://portal.ais-info.co.jp/')

    try:
        page.query_selector('#emp_id')
        page.fill('#emp_id', AIS_ID)
        page.query_selector('#emp_pwd')
        page.fill('#emp_pwd', AIS_PWD)
        page.wait_for_selector("button.btn.btn-lg.btn-primary.btn-block")
        page.click("button.btn.btn-lg.btn-primary.btn-block")
        time.sleep(1)
        print('Login succeed')
    except Exception as e:
        print(f'Login failed: {e}')
        exit()

    time.sleep(100)
    browser.close()