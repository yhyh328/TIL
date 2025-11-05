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
    port: 5432,          // ⬅️ use 5433
    ssl: false,
    connectionTimeoutMillis: 5000,
    idleTimeoutMillis: 10000,
    max: 10,
  });

pool.on("error", (err) => {
console.error("PG Pool error:", err);
});

// 라우트
app.post("/login", async (req, res) => {
    try {
      // accept both shapes for safety
      const reqID = req.body.reqID ?? req.body.id;
      const reqPW = req.body.reqPW ?? req.body.pw;
  
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

const allGetQuery   = "SELECT * FROM fruit_world ORDER BY fruit_id";
const singleGetQUERY= "SELECT * FROM fruit_world WHERE fruit_id = $1";
const postQuery     = "INSERT INTO fruit_world (fruit_id, fruit_img) VALUES ($1, $2)";
const deleteQuery   = "DELETE FROM fruit_world WHERE fruit_id = $1";

app.get("/fruit/all", async (req, res) => {
  try {
    const { rows } = await pool.query(allGetQuery);
    return res.json(rows);
  } catch (e) {
    console.error(e);
    return res.status(500).json({ error: "DB error" });
  }
});

app.get("/fruit/:id", async (req, res) => {
  try {
    const { rows } = await pool.query(singleGetQUERY, [req.params.id]);
    if (rows.length === 0) return res.status(404).json({ error: "not found" });
    return res.json(rows[0].fruit_img);
  } catch (e) {
    console.error(e);
    return res.status(500).json({ error: "DB error" });
  }
});

app.post("/fruit/reg", async (req, res) => {
  try {
    const { id, img } = req.body;
    if (!id || !img) return res.status(400).json({ error: "id and img required" });
    const r = await pool.query(postQuery, [id, img]);
    if (r.rowCount === 1) {
      const { rows } = await pool.query(allGetQuery);
      return res.json(rows);
    }
    return res.status(400).json({ error: "insert failed" });
  } catch (e) {
    console.error(e);
    return res.status(500).json({ error: "DB error" });
  }
});

app.delete("/fruit/del/:id", async (req, res) => {
  try {
    const r = await pool.query(deleteQuery, [req.params.id]);
    if (r.rowCount === 1) {
      const { rows } = await pool.query(allGetQuery);
      return res.json(rows);
    }
    return res.status(404).json({ error: "not found" });
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
