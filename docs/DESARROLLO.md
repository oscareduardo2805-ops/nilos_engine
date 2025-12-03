# Development Guide

Quick reference for developing with Nilos Engine in Cursor/VS Code.

## 🚀 Comandos Rápidos desde Terminal

### Compilar

```powershell
# Compilar (Release)
.\build.ps1

# Compilar (Debug con símbolos)
.\build.ps1 -Debug

# Limpiar y compilar desde cero
.\build.ps1 -Clean

# Compilar y ejecutar
.\build.ps1 -Run
```

### Ejecutar

```powershell
# Ejecutar (Release)
.\run.ps1

# Ejecutar (Debug)
.\run.ps1 -Debug
```

---

## 🎯 Usando Cursor/VS Code

### Opción 1: Terminal Integrada

1. **Abrir terminal**: `` Ctrl + ` `` (backtick)
2. **Compilar**: `.\build.ps1`
3. **Ejecutar**: `.\run.ps1`

Verás todos los logs en tiempo real:
```
[12:34:56.789] [INFO] === Nilos Engine Initialization ===
[12:34:56.790] [INFO] Time system initialized
[12:34:56.791] [INFO] Event system initialized
[12:34:56.850] [INFO] Window created: 1280x720
...
```

### Opción 2: Tareas de VS Code

1. **Abrir Command Palette**: `Ctrl + Shift + P`
2. Escribe: `Tasks: Run Task`
3. Selecciona:
   - **Build Nilos Engine (Release)** - Compilar
   - **Build and Run** - Compilar y ejecutar
   - **Run Nilos Engine** - Solo ejecutar
   - **Clean Build** - Limpiar todo

### Opción 3: Atajos de Teclado

- **Compilar**: `Ctrl + Shift + B` (build)
- **Ejecutar con Debug**: `F5`
- **Ejecutar sin Debug**: `Ctrl + F5`

---

## 🐛 Debugging

### Debugging Visual en Cursor

1. **Poner breakpoints**: Click en el margen izquierdo del código
2. **Iniciar debug**: `F5`
3. **Controles**:
   - `F5` - Continue
   - `F10` - Step Over
   - `F11` - Step Into
   - `Shift + F11` - Step Out
   - `Shift + F5` - Stop

### Ver Variables

Durante debug puedes ver:
- **Variables locales**: Panel izquierdo
- **Watch**: Agrega expresiones personalizadas
- **Call Stack**: Ver la pila de llamadas
- **Console**: Ejecutar comandos

---

## 📊 Viendo Logs

Los logs del engine se muestran en la terminal con el formato:

```
[HH:MM:SS.mmm] [NIVEL] Mensaje
```

Niveles disponibles:
- `TRACE` - Detalles de ejecución (muy verbose)
- `DEBUG` - Información de debug
- `INFO` - Información general
- `WARNING` - Advertencias
- `ERROR` - Errores recuperables
- `CRITICAL` - Errores críticos

### Cambiar nivel de log

En `src/Core/Engine.cpp`, al inicio de `Initialize()`:

```cpp
// Para ver más detalles
Logger::Get().SetLogLevel(LogLevel::Debug);  // o Trace

// Para ver menos
Logger::Get().SetLogLevel(LogLevel::Warning);
```

---

## 🔧 Desarrollo Típico

### 1. Hacer Cambios

Edita archivos en `src/`:

```cpp
// src/Core/Engine.cpp
void Engine::SetupDemoScene() {
    // Agrega tu código aquí
    NILOS_INFO("Creating custom entity...");
    
    auto myEntity = m_World->CreateEntity("MyEntity");
    // ...
}
```

### 2. Compilar

```powershell
.\build.ps1
```

Si hay errores, aparecerán en la terminal con el archivo y línea.

### 3. Ejecutar y Ver Logs

```powershell
.\run.ps1
```

Verás tu log:
```
[12:34:56.123] [INFO] Creating custom entity...
```

### 4. Iterar

Repite: Editar → Compilar → Ejecutar

---

## 🎨 Agregar Nuevos Componentes

### 1. Definir Componente

En `src/ECS/Component.h`:

```cpp
struct MyComponent {
    float MyValue = 0.0f;
    std::string MyData;
};
```

### 2. Usar en Entidad

En `src/Core/Engine.cpp`:

```cpp
auto entity = m_World->CreateEntity("Test");
auto* myComp = m_World->AddComponent<MyComponent>(entity);
myComp->MyValue = 42.0f;
```

### 3. Procesar en Sistema

Crear nuevo sistema o usar Update():

```cpp
void Engine::Update(float deltaTime) {
    auto entities = m_World->GetEntitiesWithComponent<MyComponent>();
    for (auto entity : entities) {
        auto* comp = m_World->GetComponent<MyComponent>(entity);
        // Hacer algo con comp
    }
}
```

---

## 🧪 Testing

### Probar Cambios Rápidamente

```powershell
# Compilar y ejecutar en un comando
.\build.ps1 -Run

# O con atajo de teclado
Ctrl + Shift + B  # Compilar
Ctrl + F5         # Ejecutar
```

### Verificar Compilación

```powershell
# Solo compilar sin ejecutar
.\build.ps1

# Ver salida detallada
# CMake muestra advertencias y errores
```

---

## ⚡ Consejos de Productividad

### 1. Usar Terminal Split

En Cursor: Click el botón `+` en la terminal
- Terminal 1: Para compilar
- Terminal 2: Para ejecutar
- Terminal 3: Para git, etc.

### 2. Atajos Útiles

- `` Ctrl + ` `` - Toggle terminal
- `Ctrl + Shift + B` - Build
- `F5` - Debug
- `Ctrl + F5` - Run sin debug
- `Ctrl + C` - Detener programa

### 3. Watch Mode (Futuro)

Para compilación automática en cambios:

```powershell
# Instalar nodemon (si tienes npm)
npm install -g nodemon

# Watch y recompilar
nodemon --watch src --exec ".\build.ps1"
```

---

## 🚨 Solución de Problemas

### "Execution Policy" Error

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### "vcpkg not found"

El script intentará compilar sin vcpkg. Asegúrate de que GLFW y GLM estén instalados.

### No se ven los logs

Verifica que estás ejecutando desde la terminal, no con doble click del archivo .exe

### Errores de compilación

```powershell
# Limpiar y recompilar
.\build.ps1 -Clean
```

---

## 📖 Recursos

- **Documentación**: Ver `docs/`
- **ECS Guide**: `docs/ECS_GUIDE.md`
- **Architecture**: `docs/ARCHITECTURE.md`
- **API Reference**: `docs/API_REFERENCE.md`

---

## 🎯 Workflow Recomendado

```
1. Abrir Cursor
2. Abrir terminal (Ctrl + `)
3. Primera vez: .\build.ps1
4. Desarrollo:
   - Editar código
   - .\build.ps1 (compilar)
   - .\run.ps1 (ejecutar y ver logs)
   - Repetir
5. Debug cuando sea necesario: F5
```

¡Happy Coding! 🚀

