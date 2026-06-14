# Flappy Bee II: The Buzz Continues 🐝

A high-performance arcade game compiled for the web using **WebAssembly (WASM)**. Built with a custom C++ game engine powered by the **Pygame++** framework wrapper and Emscripten under the hood.

## 🚀 Play the Game
Experience native C++ performance directly in your browser:
👉 **[Play Flappy Bee II Live](https://xlewis1.github.io/FlappyBee2/)**

*Note: Due to the high-quality assets and engine multi-threading compiled into the WASM bundle, it may take a moment to compile and load on the first launch!*

## 🕹️ How to Play
* **Choose Your Character:** Select your playable character from the roster, including **Bumble Bee**, **Razor Wasp**, or **Jag**!
* **The Mission:** Navigate through the environment, collect as much **honey** as you can to boost your score, and survive!
* **Avoid Obstacles:** Watch out! You must dodge **small, large, and dynamically growing flower obstacles** that block your path.

## 🛠️ Features
* **Native C++ Engine:** Powered by a custom `Pygame++` wrapper layer.
* **WebAssembly Deployment:** Compiled down to machine bytecode using Emscripten for near-native browser speeds.
* **Asset Loading with COI:** Utilizes Cross-Origin Isolation via service workers to manage secure memory allocation smoothly.
* **Showcase Logic:** Open-source `.cpp` files are included to demonstrate the gameplay loop, entity handling, and menu states.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
