const shortcuts = [
  { label: "My Home", icon: "C", accent: "#35506b" },
  { label: "ChatGPT", icon: "◎", accent: "#f3f0f7" },
  { label: "YouTube", icon: "▶", accent: "#ff3b35" },
  { label: "Feed", icon: "in", accent: "#2680c2" },
  { label: "Amazon.com", icon: "a", accent: "#f2f2f0" },
  { label: "W3Schools On...", icon: "W³", accent: "#2ea56a" },
  { label: "(8) RDCgaming", icon: "t", accent: "#8e5cff" },
  { label: "Find Cruises fr...", icon: "♛", accent: "#18307f" },
  { label: "Expedia Travel", icon: "↗", accent: "#f4d23b" },
  { label: "Add shortcut", icon: "+", accent: "#6d6375" },
];

function createIcon(name, className = "") {
  const span = document.createElement("span");
  span.className = `material-symbols-rounded ${className}`;
  span.textContent = name;
  return span;
}

function buildPage() {
  const app = document.querySelector("#app");

  app.innerHTML = `
    <header class="top-nav" aria-label="Google navigation">
      <a href="#">Gmail</a>
      <a href="#">Images</a>
      <button class="icon-button" aria-label="Labs">
        <span class="material-symbols-rounded">science</span>
      </button>
      <button class="icon-button" aria-label="Google apps">
        <span class="material-symbols-rounded">apps</span>
      </button>
      <button class="avatar" aria-label="Google account">E</button>
    </header>

    <main class="home">
      <h1 class="logo" aria-label="Google">Google</h1>

      <form class="search" role="search">
        <span class="material-symbols-rounded search-icon">search</span>
        <input aria-label="Search" type="search" placeholder="Search Google or type a URL" />
        <div class="search-actions">
          <button type="button" class="icon-button dark" aria-label="Voice search">
            <span class="material-symbols-rounded">mic</span>
          </button>
          <button type="button" class="icon-button dark" aria-label="Search by image">
            <span class="material-symbols-rounded">center_focus_strong</span>
          </button>
          <button type="button" class="ai-button" aria-label="AI Mode">
            <span class="material-symbols-rounded">auto_awesome</span>
            AI Mode
          </button>
        </div>
      </form>

      <section class="shortcuts" aria-label="Shortcuts"></section>
    </main>

    <button class="customize" type="button">
      <span class="material-symbols-rounded">edit</span>
      Customize Chrome
    </button>
  `;

  const shortcutList = app.querySelector(".shortcuts");

  shortcuts.forEach((shortcut) => {
    const item = document.createElement("button");
    item.className = "shortcut";
    item.type = "button";
    item.innerHTML = `
      <span class="shortcut-circle">
        <span class="shortcut-icon">${shortcut.icon}</span>
      </span>
      <span class="shortcut-label">${shortcut.label}</span>
    `;

    item.querySelector(".shortcut-icon").style.color = shortcut.accent;
    if (shortcut.label === "ChatGPT" || shortcut.label === "Amazon.com") {
      item.querySelector(".shortcut-icon").style.color = "#202124";
    }

    shortcutList.appendChild(item);
  });

  app.querySelector(".search").addEventListener("submit", (event) => {
    event.preventDefault();
    const query = event.currentTarget.querySelector("input").value.trim();

    if (query) {
      window.location.href = `https://www.google.com/search?q=${encodeURIComponent(query)}`;
    }
  });
}

buildPage();
