import { useState, useEffect, useRef } from 'react'
import { Trash2, Send } from 'lucide-react'
import { getComments, addComment, deleteComment } from '../api'
import { timeAgo } from '../utils'
import Avatar from './Avatar'

export default function CommentSection({ postId, onCountChange }) {
  const [comments, setComments] = useState([])
  const [loading, setLoading] = useState(true)
  const [text, setText] = useState('')
  const [submitting, setSubmitting] = useState(false)
  const inputRef = useRef(null)

  useEffect(() => {
    getComments(postId)
      .then(res => setComments(res.data.comments))
      .catch(() => {})
      .finally(() => setLoading(false))
  }, [postId])

  const handleSubmit = async (e) => {
    e.preventDefault()
    const trimmed = text.trim()
    if (!trimmed) return
    setSubmitting(true)
    try {
      const res = await addComment(postId, trimmed)
      setComments(prev => [...prev, res.data])
      onCountChange(prev => prev + 1)
      setText('')
    } catch {}
    finally { setSubmitting(false) }
  }

  const handleDelete = async (commentId) => {
    try {
      await deleteComment(commentId)
      setComments(prev => prev.filter(c => c.id !== commentId))
      onCountChange(prev => prev - 1)
    } catch {}
  }

  return (
    <div className="border-t border-gray-100 dark:border-gray-700 pt-3 mt-1">
      {loading ? (
        <p className="text-xs text-gray-400 px-4 pb-3">Loading comments…</p>
      ) : comments.length === 0 ? (
        <p className="text-xs text-gray-400 dark:text-gray-500 px-4 pb-3">No comments yet. Be the first!</p>
      ) : (
        <ul className="space-y-3 px-4 pb-3 max-h-48 overflow-y-auto">
          {comments.map(comment => (
            <li key={comment.id} className="flex gap-2 items-start group">
              <Avatar
                src={comment.avatar_url}
                name={comment.display_name}
                userId={comment.user_id}
                size="sm"
              />
              <div className="flex-1 min-w-0">
                <span className="text-xs font-semibold text-gray-800 dark:text-gray-200">{comment.display_name} </span>
                <span className="text-xs text-gray-700 dark:text-gray-300">{comment.content}</span>
                <p className="text-xs text-gray-400 dark:text-gray-500 mt-0.5">{timeAgo(comment.created_at)}</p>
              </div>
              {comment.is_owner && (
                <button
                  onClick={() => handleDelete(comment.id)}
                  className="opacity-0 group-hover:opacity-100 text-gray-400 hover:text-red-500 transition-opacity flex-shrink-0"
                >
                  <Trash2 size={13} />
                </button>
              )}
            </li>
          ))}
        </ul>
      )}

      <form onSubmit={handleSubmit} className="flex items-center gap-2 px-4 pb-3">
        <input
          ref={inputRef}
          type="text"
          value={text}
          onChange={e => setText(e.target.value)}
          placeholder="Write a comment…"
          className="flex-1 text-sm border border-gray-200 dark:border-gray-600 bg-white dark:bg-gray-700 text-gray-900 dark:text-gray-100 placeholder-gray-400 dark:placeholder-gray-500 rounded-full px-3 py-1.5 focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent"
        />
        <button
          type="submit"
          disabled={submitting || !text.trim()}
          className="text-blue-600 dark:text-blue-400 hover:text-blue-700 disabled:opacity-40 disabled:cursor-not-allowed"
        >
          <Send size={16} />
        </button>
      </form>
    </div>
  )
}
