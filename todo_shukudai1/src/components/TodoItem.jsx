import React from "react"
export default function TodoItem({todos, slashTodo}) {
    const handleSlashTodo = (todo) => slashTodo(todo) 
    return(
        <>
            {todos.map((todo) => (
                <li>
                    <a style={{textDecoration : todo.style}}>{todo.content}</a>
                    <button onClick={() => handleSlashTodo(todo)}>削除</button>
                </li>
            ))}
        </>
    )
}