import { useState } from "react";

const TodoInput = ({addTodo}) => {
  const [todoContent, setTodoContent] = useState("");
  const handleAdd = () => {
    addTodo(todoContent);
    setTodoContent("");
  }
  return (
    <>
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