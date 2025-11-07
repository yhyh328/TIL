import { BrowserRouter, Routes, Route } from "react-router-dom";
import LoginPage from "./pages/LoginPage";
import TodoPage from "./pages/TodoPage";
import "./App.css";

function App() {
  return (
    <>
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<LoginPage />} />
          <Route path="/todo_page" element={<TodoPage />} />
          <Route path="/*" element={"登録されなかったURLです。"} />
        </Routes>
      </BrowserRouter>
    </>
  );
}

export default App;
