import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'

const colors = ["black", "green", "red", "yellow", "blue", "gray", "orange", "purple", "pink"]

function App() {
  const [count, setCount] = useState(0)
  const testFunc = () => {
    console.log("The Button is clicked");
    setCount((count) => count + 1);
  }

  const [index, setIndex] = useState(0);
  const [preColor, setPreColor] = useState(colors[index]);
  const [curColor, setCurColor] = useState(colors[index]);
  useEffect(() => { setPreColor(curColor); setCurColor(colors[index]); }, [index]);

  return (
    <>
      <div>
        <span>前の色 : </span>
        <span style={{color: preColor}}>{preColor}</span>
      </div>
        <span>今の色 : </span>
        <span style={{color: curColor}}>{curColor}</span>
      <div>
        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={() => { testFunc(); setIndex((index) => (index + 1) % colors.length); }} id="counter">
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