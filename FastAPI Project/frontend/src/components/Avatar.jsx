import { getInitials, getAvatarColor } from '../utils'

const sizeClasses = {
  sm: 'w-7 h-7 text-xs',
  md: 'w-8 h-8 text-xs',
  lg: 'w-10 h-10 text-sm',
}

export default function Avatar({ src, name, userId, size = 'md' }) {
  const cls = sizeClasses[size] ?? sizeClasses.md
  const seed = userId || name || 'U'

  if (src) {
    return (
      <img
        src={src}
        alt={name}
        className={`${cls} rounded-full object-cover flex-shrink-0`}
      />
    )
  }

  return (
    <div
      className={`${cls} rounded-full flex-shrink-0 flex items-center justify-center text-white font-semibold ${getAvatarColor(seed)}`}
    >
      {getInitials(name || 'U')}
    </div>
  )
}
