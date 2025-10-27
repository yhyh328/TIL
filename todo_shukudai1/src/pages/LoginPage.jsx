import React, { useState } from "react"
import { useNavigate } from "react-router-dom";
export default function LoginPage() {
    const [registeredID, registeredPW] = ["test", "test"];
    const [currentID, setCurrentID] = useState("");
    const [currentPW, setCurrentPW] = useState("");
    const [loginFailed, setLoginFailed] = useState(false);
    const navigate = useNavigate();
    const tryLogin = () => {
        (currentID === registeredID && currentPW === registeredPW) ?
        navigate("/todo_page", {state: {userId: registeredID}}) :
        setLoginFailed(true); 
    }
    return (
        <>
            <h3>Login Page</h3>
            <input 
                type="text" 
                onChange={(e) => setCurrentID(e.target.value)} 
                placeholder="IDを入力してください。"
            />

            <input 
                type="password"
                onChange={(e) => setCurrentPW(e.target.value)} 
                placeholder="PWを入力してください。"
            />
            <button onClick={tryLogin}>ログイン試す</button>
            { loginFailed && <div>存在しないIDや間違ったPWです。</div> }
        </>
      )
}