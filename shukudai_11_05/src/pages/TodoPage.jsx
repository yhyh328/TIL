import { useState, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import TodoInput from "../components/TodoInput";
import TodoItem from "../components/TodoItem";
import axios from "axios";

const TodoPage = () => {

  const navigate = useNavigate();
  const goBack = () => navigate(-1);
  const location = useLocation();
  const msg = location.state?.msg || "error";

  if (msg === "error") { return(<>Error Occured!</>); }

  const [todos, setTodos] = useState([]);

  const url = "http://localhost:5000"

  const getTodos = async() => {
      try {
        const response = await axios.get(`${url}/get_todos`)
        const data = response.data
        setTodos(data);
      } catch (e) {
        console.error(e);
      }
    }

  const slashTodo = async (todoID) => {

    let i = 0;
    while (i < todos.length) {
      let j = todos[i].article_id;
      if (j === todoID) {
        if (todos[i].article_author !== msg) {
          console.log("作成者しか削除できません。")
          alert("作成者しか削除できません。")
          return;
        } 
      }
      i++;
    }

    setTodos(prev =>
    prev.map(t =>
      t.article_id === todoID ? { ...t, article_is_deleted: true } : t
    ));

    try {
        await axios.post(`${url}/del_todo/${todoID}`, {id : todoID });
        await getTodos();
    } catch (e) {
        console.error("Error occurs during getting rid of a todo...")
        await getTodos();
    } 
  };

  useEffect(() => { getTodos(); }, [])

  const addTodo = async (curTitle, curContent) => {
    const curAuthor = msg;
    const temp = [...todos];
    const curID = temp.length + 1;
    temp.push(
      {
       "article_id": curID,
       "article_title" : curTitle,
       "article_content" : curContent,
       "article_author": curAuthor}
    )
    setTodos(temp);
    try {
        await axios.post(`${url}/reg_todo`, {title : curTitle, content: curContent, author: curAuthor });
        await getTodos();
    } catch (e) {
        console.error("Error occurs during getting rid of a todo...")
        await getTodos();
    } 
  }

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