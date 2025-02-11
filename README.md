# Simple TCP Chatroom

> A lightweight terminal-based TCP chatroom implementation with support for multiple clients.

## Features

* Multi-client support (up to 10 concurrent users)
* Real-time message broadcasting
* User join/leave notifications
* Username display for each message
* Graceful exit handling

### Compilation

**Server:**
```bash
gcc chatserver.c -o server
```

**Client:**
```bash
gcc chatclient.c -o client
```

### Running

1. **Start the server:**
   ```bash
   ./server
   ```

2. **Connect clients:**
   ```bash
   ./client
   ```
   Enter your name when prompted:
   ```
   =========== CHATROOM ===========
   Name: YourName
   ```

## Usage

### Chat Commands

| Command | Description |
|---------|-------------|
| `Any text` | Send message to all users |
| `exit` | Leave the chatroom |
