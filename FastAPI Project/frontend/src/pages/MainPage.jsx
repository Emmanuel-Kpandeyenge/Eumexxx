import { useState, useEffect } from 'react'
import Navbar from '../components/Navbar'
import PostCard from '../components/PostCard'
import UploadPanel from '../components/UploadPanel'
import { getFeed, getMe } from '../api'

export default function MainPage() {
  const [posts, setPosts] = useState([])
  const [currentUser, setCurrentUser] = useState(null)
  const [loading, setLoading] = useState(true)
  const [activeTab, setActiveTab] = useState('feed')

  useEffect(() => {
    Promise.all([getFeed(), getMe()])
      .then(([feedRes, meRes]) => {
        setPosts(feedRes.data.posts)
        setCurrentUser(meRes.data)
      })
      .catch(() => {
        localStorage.removeItem('token')
        window.location.href = '/auth'
      })
      .finally(() => setLoading(false))
  }, [])

  const handleUploaded = (newPost) => {
    const enriched = {
      ...newPost,
      display_name: currentUser
        ? (currentUser.first_name && currentUser.last_name
            ? `${currentUser.first_name} ${currentUser.last_name}`
            : currentUser.email)
        : 'You',
      avatar_url: currentUser?.avatar_url || null,
      user_id: currentUser?.id || '',
      like_count: 0,
      comment_count: 0,
      liked_by_me: false,
      is_owner: true,
    }
    setPosts(prev => [enriched, ...prev])
    setActiveTab('feed')
  }

  const handleDelete = (postId) => {
    setPosts(prev => prev.filter(p => p.id !== postId))
  }

  const handleAvatarChange = (newAvatarUrl) => {
    setCurrentUser(prev => ({ ...prev, avatar_url: newAvatarUrl }))
  }

  // feed is "big" when activeTab === 'feed', upload is "big" when activeTab === 'upload'
  const feedBig = activeTab === 'feed'

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-950">
      <Navbar
        currentUser={currentUser}
        activeTab={activeTab}
        onTabChange={setActiveTab}
        onAvatarChange={handleAvatarChange}
      />

      <div className="max-w-6xl mx-auto px-4 py-6 flex gap-5 items-start">

        {/* Feed column */}
        <section
          className="min-w-0 transition-all duration-300 ease-in-out"
          style={{ flex: feedBig ? 3 : 1 }}
        >
          <h1 className="text-lg font-semibold text-gray-900 dark:text-gray-100 mb-4">Feed</h1>

          {loading ? (
            <div className="space-y-4">
              {[1, 2, 3].map(i => (
                <div key={i} className="bg-white dark:bg-gray-800 rounded-xl border border-gray-100 dark:border-gray-700 h-36 animate-pulse" />
              ))}
            </div>
          ) : posts.length === 0 ? (
            <div className="bg-white dark:bg-gray-800 rounded-xl border border-gray-100 dark:border-gray-700 shadow-sm p-12 text-center">
              <p className="text-gray-500 dark:text-gray-400 text-sm">No posts yet. Be the first to share something!</p>
            </div>
          ) : (
            <div className="space-y-4">
              {posts.map(post => (
                <PostCard
                  key={post.id}
                  post={post}
                  onDelete={handleDelete}
                  compact={!feedBig}
                />
              ))}
              <p className="text-center text-xs text-gray-400 dark:text-gray-600 py-4">No more posts to show</p>
            </div>
          )}
        </section>

        {/* Upload column */}
        <aside
          className="min-w-0 transition-all duration-300 ease-in-out"
          style={{ flex: feedBig ? 1 : 3 }}
        >
          <UploadPanel onUploaded={handleUploaded} compact={feedBig} />
        </aside>

      </div>
    </div>
  )
}
