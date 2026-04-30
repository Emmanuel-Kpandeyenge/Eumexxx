import axios from 'axios'

const api = axios.create({ baseURL: 'http://localhost:8000' })

api.interceptors.request.use(config => {
  const token = localStorage.getItem('token')
  if (token) config.headers.Authorization = `Bearer ${token}`
  return config
})

export const login = (email, password) => {
  const form = new URLSearchParams()
  form.append('username', email)
  form.append('password', password)
  return api.post('/auth/jwt/login', form)
}

export const register = (email, password, firstName, lastName) =>
  api.post('/auth/register', {
    email,
    password,
    first_name: firstName,
    last_name: lastName,
  })

export const getMe = () => api.get('/users/me')
export const getFeed = () => api.get('/feed')

export const uploadPost = (file, caption) => {
  const form = new FormData()
  form.append('file', file)
  form.append('caption', caption)
  return api.post('/upload', form)
}

export const deletePost = (id) => api.delete(`/posts/${id}`)
export const likePost = (id) => api.post(`/posts/${id}/like`)
export const unlikePost = (id) => api.delete(`/posts/${id}/like`)
export const getComments = (id) => api.get(`/posts/${id}/comments`)
export const addComment = (id, content) => api.post(`/posts/${id}/comments`, { content })
export const deleteComment = (id) => api.delete(`/comments/${id}`)

export const uploadAvatar = (file) => {
  const form = new FormData()
  form.append('file', file)
  return api.post('/users/me/avatar', form)
}

export default api
