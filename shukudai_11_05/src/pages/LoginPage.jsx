import { useState } from "react";
import { useNavigate } from "react-router-dom";
import axios from "axios";

const LoginPage = () => {
  const [curId, setCurId] = useState();
  const [curPw, setCurPw] = useState();
  const [isWrong, setIsWrong] = useState(false);
  const navigate = useNavigate();

  const url = "http://localhost:5000"

  const checkInfo = async () => {
    try {
        const { data } = await axios.post(`${url}/login`, {
            id: curId, pw: curPw
        });
        navigate("/todo_page", { state: { msg: data.user } }); 
    } catch {
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
