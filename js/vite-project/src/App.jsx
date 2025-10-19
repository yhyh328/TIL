import './App.css'

function App() {
  let msg = "";
  const handleChange = (e) => { msg = e.target.value; } 
  const print = () => { console.log(msg) }
  return (
    <>
      <input type="text" onChange={handleChange}/>
      <button onClick={print}>Print</button>
    </>
  )
}

export default App