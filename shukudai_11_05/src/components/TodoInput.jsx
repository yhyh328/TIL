import { useState } from "react";

const TodoInput = ({addTodo}) => {
  const [todoTitle, setTodoTitle] = useState("");
  const [todoContent, setTodoContent] = useState("");
  const handleAdd = () => {
    addTodo(todoTitle, todoContent);
    setTodoTitle("");
    setTodoContent("");
  }
  return (
    <>
        <input 
          type="text" 
          value={todoTitle} 
          onChange={(e) => setTodoTitle(e.target.value)}
        />
        <input 
          type="text" 
          value={todoContent} 
          onChange={(e) => setTodoContent(e.target.value)}
        />
        <button onClick={handleAdd}>登録</button>
    </>
  );
};

export default TodoInput;