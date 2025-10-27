import React, { useState } from "react"
export default function TodoInput({addTodo}) {
    const [todoContent, setTodoContent] = useState("")
    const handleAddTodo = () => {
        if (todoContent.trim() === "") { 
            alert("空白は入れません。")
            return
        }
        const todo = { style: "", content: todoContent }
        addTodo(todo);
        setTodoContent("");
    }
    return(
        <>
            <input 
                type="text" 
                placeholder="日程を入力してください。"
                value={todoContent}
                onChange={(e) => setTodoContent(e.target.value)}
            />
            <button onClick={handleAddTodo}>登録</button>
        </>
    )
}