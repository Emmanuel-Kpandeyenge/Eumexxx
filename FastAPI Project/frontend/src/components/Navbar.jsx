import { useState, useRef, useEffect } from 'react'
import { Bell, StickyNote, ChevronDown, Sun, Moon, Camera } from 'lucide-react'
import Avatar from './Avatar'
import { uploadAvatar } from '../api'
import useDarkMode from '../hooks/useDarkMode'

export default function Navbar({ currentUser, activeTab, onTabChange, onAvatarChange }) {
  const [dropdownOpen, setDropdownOpen] = useState(false)
  const [uploading, setUploading] = useState(false)
  const [isDark, toggleDark] = useDarkMode()
  const dropdownRef = useRef(null)
  const fileInputRef = useRef(null)

  const displayName = currentUser
    ? (currentUser.first_name && currentUser.last_name
        ? `${currentUser.first_name} ${currentUser.last_name}`
        : currentUser.email)
    : ''

  useEffect(() => {
    function handleClick(e) {
      if (dropdownRef.current && !dropdownRef.current.contains(e.target)) {
        setDropdownOpen(false)
      }
    }
    document.addEventListener('mousedown', handleClick)
    return () => document.removeEventListener('mousedown', handleClick)
  }, [])

  const handleAvatarFileChange = async (e) => {
    const file = e.target.files[0]
    if (!file) return
    setUploading(true)
    try {
      const res = await uploadAvatar(file)
      onAvatarChange(res.data.avatar_url)
    } catch {}
    finally {
      setUploading(false)
      e.target.value = ''
    }
  }

  const handleLogout = () => {
    localStorage.removeItem('token')
    window.location.href = '/auth'
  }

  return (
    <nav className="bg-white dark:bg-gray-900 border-b border-gray-200 dark:border-gray-700 px-6 h-14 flex items-center justify-between sticky top-0 z-10">
      <div className="flex items-center gap-2">
        <StickyNote size={20} className="text-blue-600" />
        <span className="font-bold text-gray-900 dark:text-gray-100 text-lg tracking-tight">Post-itz</span>
      </div>

      <div className="flex items-center border-b-2 border-transparent gap-1">
        <button
          onClick={() => onTabChange('feed')}
          className={`px-5 py-1 text-sm font-medium transition-colors ${
            activeTab === 'feed'
              ? 'text-blue-600 border-b-2 border-blue-600 -mb-px'
              : 'text-gray-500 dark:text-gray-400 hover:text-gray-700 dark:hover:text-gray-200'
          }`}
        >
          Feed
        </button>
        <button
          onClick={() => onTabChange('upload')}
          className={`px-5 py-1 text-sm font-medium transition-colors ${
            activeTab === 'upload'
              ? 'text-blue-600 border-b-2 border-blue-600 -mb-px'
              : 'text-gray-500 dark:text-gray-400 hover:text-gray-700 dark:hover:text-gray-200'
          }`}
        >
          Upload
        </button>
      </div>

      <div className="flex items-center gap-3">
        <button
          onClick={toggleDark}
          className="text-gray-500 dark:text-gray-400 hover:text-gray-700 dark:hover:text-gray-200 transition-colors"
          title={isDark ? 'Light mode' : 'Dark mode'}
        >
          {isDark ? <Sun size={18} /> : <Moon size={18} />}
        </button>

        <button className="text-gray-500 dark:text-gray-400 hover:text-gray-700 dark:hover:text-gray-200">
          <Bell size={20} />
        </button>

        <div className="relative" ref={dropdownRef}>
          <button
            onClick={() => setDropdownOpen(v => !v)}
            className="flex items-center gap-1.5"
          >
            <Avatar
              src={currentUser?.avatar_url}
              name={displayName}
              userId={currentUser?.id}
              size="md"
            />
            <ChevronDown size={14} className="text-gray-500 dark:text-gray-400" />
          </button>

          {dropdownOpen && (
            <div className="absolute right-0 top-11 bg-white dark:bg-gray-800 border border-gray-200 dark:border-gray-700 rounded-xl shadow-lg py-1 w-48 z-20">
              <div className="px-3 py-2 border-b border-gray-100 dark:border-gray-700">
                <p className="text-sm font-medium text-gray-900 dark:text-gray-100 truncate">{displayName}</p>
                <p className="text-xs text-gray-500 dark:text-gray-400 truncate">{currentUser?.email}</p>
              </div>

              <button
                onClick={() => fileInputRef.current?.click()}
                disabled={uploading}
                className="w-full flex items-center gap-2 px-3 py-2 text-sm text-gray-700 dark:text-gray-300 hover:bg-gray-50 dark:hover:bg-gray-700 transition-colors disabled:opacity-50"
              >
                <Camera size={14} />
                {uploading ? 'Uploading…' : 'Change Photo'}
              </button>
              <input
                ref={fileInputRef}
                type="file"
                accept="image/*"
                className="hidden"
                onChange={handleAvatarFileChange}
              />

              <button
                onClick={handleLogout}
                className="w-full text-left px-3 py-2 text-sm text-red-600 dark:text-red-400 hover:bg-red-50 dark:hover:bg-red-900/20 transition-colors"
              >
                Log Out
              </button>
            </div>
          )}
        </div>
      </div>
    </nav>
  )
}
