# Torrot Muvi Controller Replacement with Kellys KLS7215N

This project documents the complete replacement of the stock Torrot Muvi electric scooter controller with a **Kellys KLS7215N sine-wave BLDC controller**. It also includes instructions for installing a **custom Arduino-based display** using a 3.5" ILI9486 TFT screen.

The original Torrot setup includes CAN-bus communication between controller, battery, and display — this project bypasses that system in favor of a simpler and fully open hardware configuration.

---

## 🔧 What’s Included

- 🧠 Detailed installation manual (Czech and English)
- 📟 Arduino display code for custom dashboard
- 💡 Blinkers, brake switch, and lights wiring logic
- 📃 Bill of Materials (BOM)
- 📸 Wiring diagrams and photos

---

## 📂 Repository Structure

docs/ # Step-by-step instructions (EN + CZ)
arduino/ # Arduino code for display and signal control
bom/ # Parts list as CSV
images/ # Schematics, pinouts, photos


---

## ⚠️ Disclaimer

This project involves **modifying the core control electronics** of an electric vehicle. Please be aware of the following:

- You perform these modifications **at your own risk**.
- Incorrect wiring or setup can result in **equipment damage**, **personal injury**, or **fire**.
- Always disconnect the battery when working on the wiring.
- Make sure you understand the function of each connection before powering anything.
- This project is provided **as-is**, with no guarantees of safety or compliance with road regulations.

If you’re unsure, seek help from someone with experience in EV electronics or automotive wiring.

---

## 🛠️ Requirements

To complete this project, you'll need:

### 🧰 Hardware
- ✅ **Kellys KLS7215N** sine wave BLDC controller
- ✅ **Arduino Mega 2560**
- ✅ **3.5" ILI9486 TFT LCD Display** (with Mega shield)
- ✅ **Relays or MOSFET modules** (for blinkers and brake light switching)
- ✅ **DC-DC converter** (72V to 12V if not already present)
- ✅ **Basic wiring materials**: wires, crimp connectors, solder, heat shrink
- ✅ **Tools**: multimeter, soldering iron, wire stripper, screwdriver, etc.

### 💻 Software
- Arduino IDE (1.x or 2.x)
- [Kelly Controller PC software](https://kellycontroller.com/pages/download)

---

## 📘 Documentation

See [docs/manual.en.md](docs/manual.en.md) for complete installation steps.  
For original Czech version, see [docs/manual.cs.md](docs/manual.cs.md).

---

## 📸 Preview

_Add wiring diagrams or photos here once available._

---

Feel free to fork or contribute!
