import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'

function App() {
  const [count, setCount] = useState(0)
  const [myID, setMyID] = useState("admin")
  const [myPW, setMyPW] = useState("qwer1234")
  const [isLoggedIn, setIsLoggedIn] = useState(false)

  const testFunc = () => {
    console.log("The Button is clicked");
    setCount((count) => count + 1)
  }

  const checkInfo = () => {
    const curID = document.getElementById("myID").value
    const curPW = document.getElementById("myPW").value
    
    if (curID === myID && curPW === myPW) {
      setIsLoggedIn(true)
      return true
    } else {
      setIsLoggedIn(false)
      return false
    }
  }

  return (
    <>
      <div>
        <div>ID  
        <input type="text" id="myID" />
        </div>
        
        <div>PW  
        <input type="password" id="myPW" />
        </div>

        <button id="loginBn" onClick={checkInfo}>try to log in</button>

        <p>{isLoggedIn ? "Login is successed" : "Need Login"}</p>

        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={testFunc} id="counter">
          count is {count}
        </button>
        <p>
          Edit <code>src/App.jsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
    </>
  )
}

export default App