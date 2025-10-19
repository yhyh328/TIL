from playwright.sync_api import sync_playwright
from dotenv import load_dotenv
import os
import time

### CONFIG ###
idID = '#ID'
pwID = 'qwer1234'
bnID = '#counter'
loginBn = '#loginBn'

load_dotenv()
myID, myPW =  os.getenv('MY_ID'), os.getenv('MY_PW')

with sync_playwright() as p:
    browser = p.chromium.launch(headless=False)
    page = browser.new_page()
    page.goto('http://localhost:5173/')

    ### Login ###
    try:
        page.query_selector('#myID')
        page.fill('#myID', myID)

        page.query_selector('#myPW')
        page.fill('#myPW', myPW)

        page.wait_for_selector(loginBn)
        page.click(loginBn)
        time.sleep(1)
        print('Login succeed')

    except Exception as e:
        print(f'Login failed: {e}')
        exit()

    ### After Login ###
    page.wait_for_selector(bnID)
    button = page.query_selector(bnID)
    print('초기:', button.text_content())
    
    # 자동으로 5번 클릭
    for i in range(5):
        button.click()
        time.sleep(1)
        new_text = button.text_content()
        print(f'클릭 {i+1}회 후: {new_text}')
    
    print('자동 클릭 완료! 브라우저를 직접 닫으세요.')
    time.sleep(10)  # 10초 후 자동 종료
    browser.close()