import { useNavigate } from "react-router-dom";
import TodoItem from "../components/TodoItem";
const TodoPage = () => {
  // const navigate = useNavigate();
  return (
    <>
      {/* <button onClick={navigate(-1)}>TodoPage</button> */}
      <TodoItem></TodoItem>
    </>
  );
};

export default TodoPage;
