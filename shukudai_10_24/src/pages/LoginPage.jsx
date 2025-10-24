import { useState } from "react";
import { useNavigate } from "react-router-dom";

const LoginPage = () => {
  const [myId, myPw] = ["id", "pw"];
  const [curId, setCurId] = useState();
  const [curPw, setCurPw] = useState();
  const [isWrong, setIsWrong] = useState(false);
  const navigate = useNavigate();
  const checkInfo = () => {
    if (myId === curId && myPw === curPw) {
      navigate("/todo_page", { state: { msg: myId } });
    } else {
      setIsWrong(true);
    }
  };
  return (
    <>
      <h3>Login Page</h3>
      <input
        type="text"
        placeholder="ID"
        onChange={(e) => setCurId(e.target.value)}
      />
      <input
        type="password"
        placeholder="PW"
        onChange={(e) => setCurPw(e.target.value)}
      />
      <button onClick={checkInfo}>Click</button>
      {isWrong && <div>Wrong ID or Password</div>}
    </>
  );
};

export default LoginPage;
