import { useState } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import TodoInput from "../components/TodoInput";
import TodoItem from "../components/TodoItem";

const TodoPage = () => {

  const navigate = useNavigate();
  const goBack = () => navigate(-1);
  const location = useLocation();
  const msg = location.state.msg;
  // const [todos, setTodos] = useState([
  //   { content: "a", style: "" },
  //   { content: "b", style: "" },
  //   { content: "c", style: "" },
  // ]);

  const [todos, setTodos] = useState([])

  const addTodo = (todoContent) => {
    setTodos([...todos, {content: todoContent, style: ""}]);
  }

  const slashTodo = (content) => {
    let i = 0;
    while (todos[i].content != content) { i++; }
    let tmp = [...todos];
    tmp[i].style = "line-through";
    setTodos(tmp);
  };

  return (
    <>
      <h3>{msg} is logged in</h3>
      <div>
        <button onClick={goBack}>Logout</button>
      </div>
      <div>
        <TodoInput addTodo={addTodo} />
        <TodoItem todos={todos} slashTodo={slashTodo}/>
      </div>
    </>
  );
};

export default TodoPage;