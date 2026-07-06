# Remote Desktop (C++ / Qt6)

A lightweight **remote desktop** application written in **C++17** using the **Qt6** framework
(Widgets + Network). It lets a controller machine view a remote machine's screen in real time
and control it with mouse and keyboard over a TCP connection.

## Architecture

The system is split into two Qt desktop applications:

| App              | Role                                | Responsibilities                                                                                                |
| ---------------- | ----------------------------------- | --------------------------------------------------------------------------------------------------------------- |
| **Server** | The machine being controlled (host) | Captures the screen, encodes it, streams it to clients, and injects incoming mouse/keyboard events into the OS. |
| **Client** | The controller (viewer)             | Connects to the server, displays the streamed screen, and captures local mouse/keyboard input to send back.     |

```
 ┌─────────── Client ───────────┐            ┌─────────── Server ───────────┐
 │  Qt GUI (QLabel display)      │  screen    │  Screen capture (Win32 GDI)  │
 │  Mouse/keyboard capture  ─────┼──events───►│  Input injection (Win32)     │
 │  Frame reassembly + display ◄─┼──JPEG──────┤  JPEG encode (worker thread) │
 │  DeviceController (QTcpSocket)│  frames    │  MyTCPServer (QTcpServer)    │
 └───────────────────────────────┘   TCP      └───────────────────────────────┘
```

## Key features

- **TCP client–server networking** with Qt's `QTcpServer` / `QTcpSocket` (supports multiple clients).
- **Screen capture** using the Win32 GDI API (`BitBlt` + `GetDIBits`) into a raw 32-bit bitmap.
- **JPEG compression** of each frame (via `QImage`) to reduce bandwidth over the network.
- **Length-prefixed message framing** to reassemble frames from a fragmented TCP byte stream.
- **Remote input injection** — mouse clicks/moves and keystrokes are replayed on the host with
  `mouse_event` / `keybd_event`, including a Qt→Win32 virtual-key mapping.
- **Multithreading** — image encoding runs on a separate `QThread` to keep the GUI responsive.
- **Event-driven design** built on Qt's signals & slots.

## Tech stack

- C++17
- Qt 6 (QtWidgets, QtNetwork, QtGui)
- Win32 API (GDI screen capture, input injection)
- Visual Studio / MSVC

## Project structure

```
Source/
  Server/   ServerTCP, MyTCPServer, processImage  — capture, encode, stream, inject input
  Client/   ClientTCP, DeviceController, Client   — connect, display, capture input
```

## Build

Open the project in Visual Studio with the Qt VS Tools extension (Qt 6 installed), or configure
with qmake/CMake against a Qt 6 kit, then build the Server and Client targets separately.

## Demo

> Demo video is too large for GitHub — add your Google Drive / YouTube link here.

## Notes & possible improvements

This was an early learning project. Known limitations / future work:

- Windows-only (Win32 API) — screen capture and input injection would need porting for Linux/macOS.
- Full frames are sent instead of dirty-region deltas; frame delivery is request/response.
- No authentication or encryption on the connection.
