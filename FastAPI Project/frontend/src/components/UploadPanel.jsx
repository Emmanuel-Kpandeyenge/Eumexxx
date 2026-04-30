import { useState, useRef } from 'react'
import { CloudUpload, X } from 'lucide-react'
import { uploadPost } from '../api'

export default function UploadPanel({ onUploaded, compact = false }) {
  const [file, setFile] = useState(null)
  const [preview, setPreview] = useState(null)
  const [caption, setCaption] = useState('')
  const [dragging, setDragging] = useState(false)
  const [uploading, setUploading] = useState(false)
  const [error, setError] = useState('')
  const inputRef = useRef(null)

  const ACCEPTED = ['image/png', 'image/jpeg', 'image/jpg', 'video/mp4']

  const handleFile = (f) => {
    if (!f) return
    if (!ACCEPTED.includes(f.type)) {
      setError('Only PNG, JPG, and MP4 files are supported.')
      return
    }
    setError('')
    setFile(f)
    setPreview(URL.createObjectURL(f))
  }

  const handleDrop = (e) => {
    e.preventDefault()
    setDragging(false)
    handleFile(e.dataTransfer.files[0])
  }

  const handleSubmit = async () => {
    if (!file) return
    setUploading(true)
    setError('')
    try {
      const res = await uploadPost(file, caption)
      onUploaded(res.data)
      setFile(null)
      setPreview(null)
      setCaption('')
    } catch (err) {
      setError(err.response?.data?.detail || 'Upload failed.')
    } finally {
      setUploading(false)
    }
  }

  const clearFile = () => {
    setFile(null)
    setPreview(null)
    setError('')
  }

  return (
    <div className="bg-white dark:bg-gray-800 rounded-xl border border-gray-100 dark:border-gray-700 shadow-sm p-5">
      <h2 className="text-base font-semibold text-gray-900 dark:text-gray-100 mb-4">Upload</h2>

      {!file ? (
        <div
          onDragOver={e => { e.preventDefault(); setDragging(true) }}
          onDragLeave={() => setDragging(false)}
          onDrop={handleDrop}
          onClick={() => inputRef.current?.click()}
          className={`border-2 border-dashed rounded-xl flex flex-col items-center justify-center cursor-pointer transition-colors ${
            compact ? 'py-6 px-3' : 'py-10 px-4'
          } ${
            dragging
              ? 'border-blue-400 bg-blue-50 dark:bg-blue-900/20'
              : 'border-gray-200 dark:border-gray-600 hover:border-gray-300 dark:hover:border-gray-500 hover:bg-gray-50 dark:hover:bg-gray-700/50'
          }`}
        >
          <CloudUpload size={compact ? 24 : 32} className="text-blue-400 mb-2" />
          <p className={`text-gray-600 dark:text-gray-400 text-center ${compact ? 'text-xs' : 'text-sm'}`}>
            Drag and drop file here<br />
            <span className="text-blue-600 dark:text-blue-400">or click to browse</span>
          </p>
          <p className="text-xs text-gray-400 dark:text-gray-500 mt-1">PNG, JPG, MP4</p>
          <input
            ref={inputRef}
            type="file"
            accept=".png,.jpg,.jpeg,.mp4"
            className="hidden"
            onChange={e => handleFile(e.target.files[0])}
          />
        </div>
      ) : (
        <div className="relative rounded-xl overflow-hidden bg-gray-100 dark:bg-gray-700 mb-3">
          {file.type.startsWith('image/') ? (
            <img src={preview} alt="Preview" className={`w-full object-cover ${compact ? 'h-28' : 'h-40'}`} />
          ) : (
            <video src={preview} className={`w-full object-cover ${compact ? 'h-28' : 'h-40'}`} />
          )}
          <button
            onClick={clearFile}
            className="absolute top-2 right-2 bg-black/50 text-white rounded-full p-1 hover:bg-black/70"
          >
            <X size={14} />
          </button>
          <div className="absolute bottom-0 left-0 right-0 bg-black/40 px-3 py-1.5">
            <p className="text-white text-xs truncate">{file.name}</p>
          </div>
        </div>
      )}

      <div className="mt-3">
        <label className="block text-sm font-medium text-gray-700 dark:text-gray-300 mb-1">Caption</label>
        <textarea
          value={caption}
          onChange={e => setCaption(e.target.value)}
          placeholder="Write a caption…"
          rows={compact ? 2 : 3}
          className="w-full border border-gray-200 dark:border-gray-600 bg-white dark:bg-gray-700 text-gray-900 dark:text-gray-100 placeholder-gray-400 dark:placeholder-gray-500 rounded-lg px-3 py-2 text-sm focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent resize-none"
        />
        <div className="flex justify-end">
          <span className="text-xs text-gray-400 dark:text-gray-500">{caption.length}/200</span>
        </div>
      </div>

      {error && <p className="text-red-500 dark:text-red-400 text-sm mt-2">{error}</p>}

      <button
        onClick={handleSubmit}
        disabled={!file || uploading}
        className="w-full mt-3 bg-blue-600 text-white py-2.5 rounded-lg font-medium hover:bg-blue-700 transition-colors disabled:opacity-50 disabled:cursor-not-allowed"
      >
        {uploading ? 'Uploading…' : 'Share Post'}
      </button>
    </div>
  )
}
