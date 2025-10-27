const TodoItem = ({todos, slashTodo}) => {
  return (
    <>
        <ul>
          {todos.map((todo) => (
            <>
            <li>
              <a style={{ textDecoration: todo.style }}>{todo.content}</a>
              <button onClick={() => slashTodo(todo.content)}>削除</button>
              </li>
            </>
          ))}
        </ul>
    </>
  );
};

export default TodoItem;