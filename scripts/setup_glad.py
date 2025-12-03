#!/usr/bin/env python3
"""
GLAD Setup Script for Nilos Engine

This script automatically downloads and sets up GLAD for OpenGL 3.3 Core.
Run this before building the project.

Usage:
    python scripts/setup_glad.py
"""

import os
import sys
import subprocess

def main():
    print("=" * 60)
    print("Nilos Engine - GLAD Setup Script")
    print("=" * 60)
    print()
    
    # Check if glad is already installed
    glad_path = os.path.join("external", "glad")
    glad_header = os.path.join(glad_path, "include", "glad", "glad.h")
    
    if os.path.exists(glad_header):
        print("✅ GLAD is already set up!")
        print(f"   Found: {glad_header}")
        response = input("\nDo you want to regenerate GLAD? (y/N): ").strip().lower()
        if response != 'y':
            print("Keeping existing GLAD setup.")
            return
        print()
    
    # Check if glad Python package is installed
    try:
        import glad
        print("✅ GLAD Python package found")
    except ImportError:
        print("⚠️  GLAD Python package not found")
        print("   Installing glad...")
        try:
            subprocess.check_call([sys.executable, "-m", "pip", "install", "glad"])
            print("✅ GLAD Python package installed")
        except subprocess.CalledProcessError:
            print("❌ Failed to install GLAD package")
            print("\nPlease install manually:")
            print("   pip install glad")
            return 1
    
    print()
    print("Generating GLAD files for OpenGL 3.3 Core...")
    print()
    
    # Generate GLAD
    try:
        cmd = [
            sys.executable, "-m", "glad",
            "--generator", "c",
            "--spec", "gl",
            "--out-path", glad_path,
            "--api", "gl=3.3"
        ]
        
        subprocess.check_call(cmd)
        print()
        print("✅ GLAD generated successfully!")
        print()
        print("Generated files:")
        print(f"   - {os.path.join(glad_path, 'include', 'glad', 'glad.h')}")
        print(f"   - {os.path.join(glad_path, 'include', 'KHR', 'khrplatform.h')}")
        print(f"   - {os.path.join(glad_path, 'src', 'glad.c')}")
        print()
        print("✅ Setup complete! You can now build the project.")
        
    except subprocess.CalledProcessError as e:
        print(f"❌ Failed to generate GLAD: {e}")
        print()
        print("Alternative: Download manually from https://glad.dav1d.de/")
        return 1
    
    return 0

if __name__ == "__main__":
    try:
        exit_code = main()
        sys.exit(exit_code if exit_code is not None else 0)
    except KeyboardInterrupt:
        print("\n\nSetup cancelled by user.")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Unexpected error: {e}")
        sys.exit(1)

