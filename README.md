# RWayLand

A lightweight **Wayland compositor** designed to provide a modern, efficient, and customizable graphical environment for Linux.

## Overview

**RWayLand** is a Wayland compositor project focused on creating a secure, flexible, and developer-friendly Linux graphics environment.

The project explores advanced Wayland compositor features, including custom window management, rendering, and enhanced application privacy controls.

## Features

* 🚀 Native Wayland compositor architecture
* 🖥️ Modern Linux display management
* 🪟 Custom window handling and composition
* ⚡ Lightweight and performance-focused design
* 🔧 Developer-friendly compositor framework
* 🔒 Improved application isolation through Wayland security model
* 🎨 Customizable desktop experience

## Privacy & Screen Share Protection

RWayLand introduces a compositor-level privacy feature designed to give applications more control over how their windows appear during screen sharing.

Traditional display systems allow screen capture tools to capture visible application content. With Wayland's compositor-based architecture, RWayLand can provide additional privacy controls by allowing windows to be marked as **capture-restricted**.

A protected window can:

* 🚫 Prevent its contents from appearing in screen sharing sessions
* 🔐 Remain hidden from remote capture tools such as conferencing applications
* 🛡️ Provide better privacy for sensitive applications
* ⚙️ Allow compositor-level control over capture permissions

This feature is designed around the Wayland security model, where the compositor controls what content is exposed to external clients.

## Why Wayland?

Wayland is a modern replacement for X11. It provides:

* Better security through application isolation
* Lower latency rendering
* Improved GPU acceleration
* More efficient graphics handling
* Stronger control over application visibility

## Project Goals

RWayLand aims to:

* Build a functional Wayland compositor from the ground up
* Explore Wayland protocols and compositor development
* Improve desktop privacy and security
* Provide advanced window management features
* Experiment with new compositor-level capabilities

## Requirements

Currently, RWayLand requires:

* Linux system with Wayland support
* Compatible GPU drivers
* Wayland development libraries

*(Requirements may change as development progresses.)*

## Installation

Clone the repository:

```bash
git clone https://github.com/yourusername/RWayLand.git
cd RWayLand
```

Build instructions will be added as development progresses.

## Development

Contributions, ideas, and feedback are welcome.

To contribute:

```bash
git checkout -b feature-name
git commit -m "Add feature"
git push
```

Then open a Pull Request.

## Roadmap

* [ ] Basic Wayland compositor initialization
* [ ] Window management
* [ ] Input handling
* [ ] Rendering pipeline
* [ ] Multi-monitor support
* [ ] Configuration system
* [ ] Window capture protection property
* [ ] Privacy-focused screen sharing controls
* [ ] Plugin/extension support

## License

License information will be added.

## Acknowledgements

Inspired by the Linux graphics stack, Wayland ecosystem, and modern compositor projects.
