const TodoItem = ({todos, slashTodo}) => {
    return (
      <>
          <ul>
            {todos.map((todo) => (
              <li key={todo.article_id}>
                <br />
                { !todo.article_is_deleted ? 
                <>
                  <div> 
                    題目　：　{todo.article_title}
                  </div>
                  <div> 内容　：　{todo.article_content}</div>
                  <button onClick={() => slashTodo(todo.article_id)}>削除</button>
                </>
                :
                <>
                  <div style={{ textDecoration: "line-through"}}> 
                    題目　：　{todo.article_title}
                  </div>
                </>
                }
                <br />
              </li>
              
            ))}
          </ul>
      </>
    );
  };
  
  export default TodoItem;