import React, { useState } from "react"
import { useNavigate, useLocation } from "react-router-dom";
import TodoInput from "../components/TodoInput";
import TodoItem from "../components/TodoItem";

export default function TodoPage() {
    
    const location = useLocation();
    const userId = location.state?.userId || null;
    if (!userId) { return "エラーが発生しました。"}

    const [todos, setTodos] = useState([]) 
    
    const addTodo = (todo) => setTodos([...todos, todo]);
    
    const slashTodo = (todo) => {
        let tmp = [...todos];
        for (let i = 0; i < todos.length; i++) {
            if (todos[i].content === todo.content) { 
                tmp[i].style = "line-through"; 
            }
        }
        setTodos(tmp);
    }

    const navigate = useNavigate()
    const logout = () => navigate(-1);

    return(
        <>
            <h3>ユーザ{userId}のリスト</h3>
            <button onClick={logout}>ログアウト</button>
            <div>
                <TodoInput addTodo={addTodo} />
                <TodoItem todos={todos} slashTodo={slashTodo} />
            </div>
        </>
   )
}