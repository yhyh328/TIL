import { useState } from "react";
const TodoItem = () => {
  const [todos, setTodos] = useState([
    { content: "a", style: "" },
    { content: "b", style: "" },
    { content: "c", style: "" },
  ]);
  // const addTodo = (todo) => setTodos(todos.push(todo));
  const deleteTodo = (content) => {
    return "line-through";
  };

  return (
    <>
      <div>
        <ul>
          {todos.map((todo) => (
            <>
              <div style={{ textDecoration: todo.style }}>{todo.content}</div>
              <button onClick={deleteTodo(todo.content)}></button>
            </>
          ))}
        </ul>
      </div>
    </>
  );
};
export default TodoItem;
