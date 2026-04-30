import { useState, useRef, useEffect } from 'react'
import { Heart, MessageCircle, MoreHorizontal, Trash2 } from 'lucide-react'
import { likePost, unlikePost, deletePost } from '../api'
import { timeAgo } from '../utils'
import Avatar from './Avatar'
import CommentSection from './CommentSection'

export default function PostCard({ post, onDelete, compact = false }) {
  const [liked, setLiked] = useState(post.liked_by_me)
  const [likeCount, setLikeCount] = useState(post.like_count)
  const [commentCount, setCommentCount] = useState(post.comment_count)
  const [showComments, setShowComments] = useState(false)
  const [menuOpen, setMenuOpen] = useState(false)
  const [deleting, setDeleting] = useState(false)
  const menuRef = useRef(null)

  useEffect(() => {
    function handleClick(e) {
      if (menuRef.current && !menuRef.current.contains(e.target)) setMenuOpen(false)
    }
    document.addEventListener('mousedown', handleClick)
    return () => document.removeEventListener('mousedown', handleClick)
  }, [])

  const handleLike = async () => {
    const wasLiked = liked
    setLiked(!wasLiked)
    setLikeCount(c => wasLiked ? c - 1 : c + 1)
    try {
      if (wasLiked) await unlikePost(post.id)
      else await likePost(post.id)
    } catch {
      setLiked(wasLiked)
      setLikeCount(c => wasLiked ? c + 1 : c - 1)
    }
  }

  const handleDelete = async () => {
    setDeleting(true)
    try {
      await deletePost(post.id)
      onDelete(post.id)
    } catch {
      setDeleting(false)
    }
  }

  const likeBtn = (
    <button
      onClick={handleLike}
      className={`flex items-center gap-1.5 text-sm transition-colors ${
        liked ? 'text-red-500' : 'text-gray-500 dark:text-gray-400 hover:text-red-400'
      }`}
    >
      <Heart size={15} className={liked ? 'fill-red-500 stroke-red-500' : ''} />
      <span>{likeCount}</span>
    </button>
  )

  const commentBtn = (
    <button
      onClick={() => setShowComments(v => !v)}
      className={`flex items-center gap-1.5 text-sm transition-colors ${
        showComments ? 'text-blue-500' : 'text-gray-500 dark:text-gray-400 hover:text-blue-500'
      }`}
    >
      <MessageCircle size={15} />
      <span>{commentCount}</span>
    </button>
  )

  if (compact) {
    return (
      <div className="bg-white dark:bg-gray-800 rounded-xl border border-gray-100 dark:border-gray-700 shadow-sm overflow-hidden">
        <div className="bg-gray-100 dark:bg-gray-700 h-32">
          {post.file_type === 'image' ? (
            <img src={post.url} alt={post.caption || ''} className="w-full h-full object-cover" />
          ) : (
            <video src={post.url} className="w-full h-full object-cover" />
          )}
        </div>
        <div className="p-3">
          <div className="flex items-center gap-2 mb-1">
            <Avatar src={post.avatar_url} name={post.display_name} userId={post.user_id} size="sm" />
            <span className="text-xs font-semibold text-gray-900 dark:text-gray-100 truncate">{post.display_name}</span>
          </div>
          {post.caption && (
            <p className="text-xs text-gray-600 dark:text-gray-400 line-clamp-1 mb-2">{post.caption}</p>
          )}
          <div className="flex items-center gap-3">
            {likeBtn}
            {commentBtn}
          </div>
        </div>
        {showComments && (
          <CommentSection postId={post.id} onCountChange={setCommentCount} />
        )}
      </div>
    )
  }

  return (
    <div className="bg-white dark:bg-gray-800 rounded-xl border border-gray-100 dark:border-gray-700 shadow-sm overflow-hidden">
      <div className="flex">
        <div className="w-44 flex-shrink-0 bg-gray-100 dark:bg-gray-700">
          {post.file_type === 'image' ? (
            <img
              src={post.url}
              alt={post.caption || 'Post image'}
              className="w-full h-full object-cover"
              style={{ minHeight: '140px', maxHeight: '180px' }}
            />
          ) : (
            <video
              src={post.url}
              className="w-full h-full object-cover"
              style={{ minHeight: '140px', maxHeight: '180px' }}
              controls
            />
          )}
        </div>

        <div className="flex-1 p-4 flex flex-col justify-between min-w-0">
          <div>
            <div className="flex items-start justify-between gap-2">
              <div className="flex items-center gap-2 min-w-0">
                <Avatar src={post.avatar_url} name={post.display_name} userId={post.user_id} size="md" />
                <div className="min-w-0">
                  <p className="text-sm font-semibold text-gray-900 dark:text-gray-100 truncate">{post.display_name}</p>
                  <p className="text-xs text-gray-400 dark:text-gray-500">{timeAgo(post.created_at)}</p>
                </div>
              </div>

              {post.is_owner && (
                <div className="relative flex-shrink-0" ref={menuRef}>
                  <button
                    onClick={() => setMenuOpen(v => !v)}
                    className="text-gray-400 hover:text-gray-600 dark:hover:text-gray-200 p-1 rounded"
                  >
                    <MoreHorizontal size={16} />
                  </button>
                  {menuOpen && (
                    <div className="absolute right-0 top-7 bg-white dark:bg-gray-800 border border-gray-200 dark:border-gray-700 rounded-xl shadow-lg py-1 w-32 z-10">
                      <button
                        onClick={handleDelete}
                        disabled={deleting}
                        className="w-full flex items-center gap-2 px-3 py-2 text-sm text-red-600 dark:text-red-400 hover:bg-red-50 dark:hover:bg-red-900/20 transition-colors disabled:opacity-50"
                      >
                        <Trash2 size={14} />
                        {deleting ? 'Deleting…' : 'Delete'}
                      </button>
                    </div>
                  )}
                </div>
              )}
            </div>

            {post.caption && (
              <p className="text-sm text-gray-700 dark:text-gray-300 mt-2 line-clamp-2">{post.caption}</p>
            )}
          </div>

          <div className="flex items-center gap-4 mt-3">
            {likeBtn}
            {commentBtn}
          </div>
        </div>
      </div>

      {showComments && (
        <CommentSection postId={post.id} onCountChange={setCommentCount} />
      )}
    </div>
  )
}
