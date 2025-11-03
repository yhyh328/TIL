import React, { useEffect, useState } from 'react'

const API = import.meta.env.VITE_API_URL || 'http://localhost:8080'

interface Memo {
  id: number
  title: string
  content: string
  createdAt: string
}

export default function App() {
  const [memos, setMemos] = useState<Memo[]>([])
  const [title, setTitle] = useState('')
  const [content, setContent] = useState('')
  const [loading, setLoading] = useState(true)

  const fetchMemos = async () => {
    try {
      const response = await fetch(`${API}/api/memos`)
      if (response.ok) {
        const data = await response.json()
        setMemos(data)
      }
    } catch (error) {
      console.error('Failed to fetch memos:', error)
    } finally {
      setLoading(false)
    }
  }

  useEffect(() => {
    fetchMemos()
  }, [])

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    if (!title.trim()) return

    try {
      const response = await fetch(`${API}/api/memos`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ title, content }),
      })

      if (response.ok) {
        setTitle('')
        setContent('')
        fetchMemos()
      }
    } catch (error) {
      console.error('Failed to create memo:', error)
    }
  }

  const handleDelete = async (id: number) => {
    try {
      const response = await fetch(`${API}/api/memos/${id}`, {
        method: 'DELETE',
      })

      if (response.ok) {
        fetchMemos()
      }
    } catch (error) {
      console.error('Failed to delete memo:', error)
    }
  }

  const formatDate = (dateString: string) => {
    const date = new Date(dateString)
    return date.toLocaleString('ko-KR', {
      year: 'numeric',
      month: '2-digit',
      day: '2-digit',
      hour: '2-digit',
      minute: '2-digit',
    })
  }

  return (
    <main style={{ maxWidth: 680, margin: '64px auto', fontFamily: 'system-ui, sans-serif', padding: '0 16px' }}>
      <h1 style={{ textAlign: 'center', marginBottom: '32px' }}>📝 메모 앱</h1>

      <form onSubmit={handleSubmit} style={{ marginBottom: '32px', padding: '20px', background: '#f5f5f5', borderRadius: '8px' }}>
        <div style={{ marginBottom: '12px' }}>
          <input
            type="text"
            placeholder="제목을 입력하세요"
            value={title}
            onChange={(e) => setTitle(e.target.value)}
            style={{
              width: '100%',
              padding: '12px',
              fontSize: '16px',
              border: '1px solid #ddd',
              borderRadius: '4px',
              boxSizing: 'border-box',
            }}
          />
        </div>
        <div style={{ marginBottom: '12px' }}>
          <textarea
            placeholder="내용을 입력하세요 (선택사항)"
            value={content}
            onChange={(e) => setContent(e.target.value)}
            rows={3}
            style={{
              width: '100%',
              padding: '12px',
              fontSize: '16px',
              border: '1px solid #ddd',
              borderRadius: '4px',
              boxSizing: 'border-box',
              resize: 'vertical',
            }}
          />
        </div>
        <button
          type="submit"
          disabled={!title.trim()}
          style={{
            width: '100%',
            padding: '12px',
            fontSize: '16px',
            background: title.trim() ? '#007bff' : '#ccc',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: title.trim() ? 'pointer' : 'not-allowed',
          }}
        >
          메모 추가
        </button>
      </form>

      <div>
        <h2 style={{ marginBottom: '16px' }}>메모 목록 ({memos.length})</h2>
        {loading ? (
          <p style={{ textAlign: 'center', color: '#666' }}>로딩 중...</p>
        ) : memos.length === 0 ? (
          <p style={{ textAlign: 'center', color: '#666', padding: '40px' }}>메모가 없습니다. 위에서 새 메모를 추가해보세요!</p>
        ) : (
          <div style={{ display: 'flex', flexDirection: 'column', gap: '16px' }}>
            {memos.map((memo) => (
              <div
                key={memo.id}
                style={{
                  padding: '20px',
                  background: 'white',
                  border: '1px solid #ddd',
                  borderRadius: '8px',
                  boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
                }}
              >
                <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'start', marginBottom: '8px' }}>
                  <h3 style={{ margin: 0, fontSize: '18px', color: '#333' }}>{memo.title}</h3>
                  <button
                    onClick={() => handleDelete(memo.id)}
                    style={{
                      background: '#dc3545',
                      color: 'white',
                      border: 'none',
                      borderRadius: '4px',
                      padding: '4px 12px',
                      cursor: 'pointer',
                      fontSize: '12px',
                    }}
                  >
                    삭제
                  </button>
                </div>
                {memo.content && (
                  <p style={{ margin: '8px 0', color: '#666', whiteSpace: 'pre-wrap' }}>{memo.content}</p>
                )}
                <p style={{ margin: '8px 0 0 0', fontSize: '12px', color: '#999' }}>
                  작성일: {formatDate(memo.createdAt)}
                </p>
              </div>
            ))}
          </div>
        )}
      </div>
    </main>
  )
}
