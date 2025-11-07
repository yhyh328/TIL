const express = require("express");
const cors = require("cors");
const { Pool } = require("pg");

const app = express();
const PORT = 5000;

// middlewares
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(cors());

// DB pool (필요시 .env로 빼도 됨)] 
const pool = new Pool({
    user: "appuser",
    host: "127.0.0.1",
    database: "shukudai_11_06",
    password: "password",
    port: 5432,          
    ssl: false,
    connectionTimeoutMillis: 5000,
    idleTimeoutMillis: 10000,
    max: 10,
  });

pool.on("error", (err) => {
console.error("PG Pool error:", err);
});


app.post("/login", async (req, res) => {
    try {
      // accept both shapes for safety
      const reqID =  req.body.id;
      const reqPW =  req.body.pw;
  
      if (!reqID || !reqPW) {
        return res.status(400).json({ ok: false, message: "id and pw are required" });
      }
  
      const { rows } = await pool.query(
        "SELECT user_pw FROM users WHERE user_id = $1",
        [reqID]
      );
  
      if (rows.length === 0) {
        return res.status(404).json({ ok: false, message: "Invalid ID" });
      }
  
      const curPW = rows[0].user_pw;
      if (curPW === reqPW) {
        return res.json({ ok: true, user: reqID });
      } else {
        return res.status(401).json({ ok: false, message: "Invalid password" });
      }
    } catch (e) {
      console.error("login error:", e);
      return res.status(500).json({ ok: false, message: "Login failed" });
    }
  });

app.get("/get_todos", async (req, res) => {
  try {
    const { rows } = await pool.query("SELECT * FROM articles");
    return res.json(rows);
  } catch (e) {
    console.error(e);
    return res.status(500).json({ error: "DB error" });
  }
});


app.post("/del_todo/:id", async (req, res) => {
    try {
      const reqID = req.body.id;
    
      if (!reqID) {
        return res.status(400).json({ ok: false, message: `Error occur -> reqID is ${reqID}` });
      }
  
      await pool.query(
        "UPDATE articles SET article_is_deleted = true WHERE article_id = $1", [reqID]
      );
  
    } catch (e) {
      console.error(e);
    }
  });

app.post("/reg_todo", async (req, res) => {
  try {
    const { title, content, author } = req.body;
    if (!title || !content || !author) return res.status(400).json({ error: "id and img required" });
    await pool.query(
      "insert into articles (article_title, article_content, article_author) values($1, $2, $3);", 
      [title, content, author]
    );
  } catch (e) {
    console.error(e);
    return res.status(500).json({ error: "DB error" });
  }
});


const server = app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});

// 종료시 정리
process.on("SIGINT", async () => {
  console.log("Shutting down...");
  await pool.end().catch(()=>{});
  server.close(() => process.exit(0));
});
