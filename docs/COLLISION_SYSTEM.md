# Sistema de Colisiones Polimórfico

## 🎯 Visión General

El motor Nilos utiliza un **sistema de colisiones universal** que garantiza que **NINGÚN objeto puede traspasar otro**, sin importar su forma geométrica.

### Principio Fundamental

**TODO → AABB → Colisión**

1. Cualquier forma (cubo, esfera, pirámide, malla compleja)
2. Se convierte a AABB (Axis-Aligned Bounding Box)
3. Las colisiones se detectan entre AABBs
4. **Resultado:** Colisión precisa y sin penetración

---

## 🏗️ Arquitectura

### Componentes Clave

```cpp
// 1. ColliderComponent - Define la forma
struct ColliderComponent {
    Type ColliderType;  // Box, Sphere, Capsule, Mesh
    glm::vec3 Size;     // Para cubos y meshes
    float Radius;       // Para esferas y cápsulas
    float Height;       // Para cápsulas
};

// 2. PhysicsWorld::GetWorldAABB() - Convierte forma → AABB
AABB GetWorldAABB(const ColliderComponent*, const TransformComponent*);

// 3. PhysicsWorld::Update() - Detecta y resuelve colisiones
void Update(float deltaTime);
```

### Flujo de Colisión

```
[Object A] ─┐
            ├─→ [GetWorldAABB] ─→ [AABB.Intersects] ─→ [Collision?]
[Object B] ─┘                                               │
                                                            ├─ YES → Resolve
                                                            └─ NO  → Skip
```

---

## 🔧 Formas Soportadas

### ✅ Box (Cubo/Rectángulo)
```cpp
collider->ColliderType = ColliderComponent::Type::Box;
collider->Size = glm::vec3(2.0f, 1.0f, 0.5f); // Ancho, Alto, Profundidad
```

### ✅ Sphere (Esfera)
```cpp
collider->ColliderType = ColliderComponent::Type::Sphere;
collider->Radius = 0.5f;
```

### ✅ Capsule (Cápsula)
```cpp
collider->ColliderType = ColliderComponent::Type::Capsule;
collider->Radius = 0.3f;  // Radio del cilindro
collider->Height = 2.0f;  // Altura total
```

### ✅ Mesh (Malla Arbitraria)
```cpp
collider->ColliderType = ColliderComponent::Type::Mesh;
collider->Size = ComputeBoundingBox(vertices); // De los vértices
```

---

## 📚 Agregar Nueva Forma (Ejemplo: Pirámide)

### Paso 1: Agregar Tipo al Enum

```cpp
// src/ECS/Component.h
struct ColliderComponent {
    enum class Type {
        Box,
        Sphere,
        Capsule,
        Mesh,
        Pyramid  // ← NUEVO
    };
    
    // Agregar parámetros necesarios
    float BaseSize = 1.0f;  // Tamaño de la base
    float ApexHeight = 2.0f; // Altura al ápice
};
```

### Paso 2: Implementar AABB en PhysicsWorld

```cpp
// src/Physics/PhysicsWorld.cpp
AABB PhysicsWorld::GetWorldAABB(...) const {
    switch (collider->ColliderType) {
        // ... casos existentes ...
        
        case ColliderComponent::Type::Pyramid: {
            // Pirámide: base en Y=0, ápice en Y=height
            float base = collider->BaseSize * glm::max(transform->Scale.x, transform->Scale.z);
            float height = collider->ApexHeight * transform->Scale.y;
            
            worldSize = glm::vec3(base, height, base);
            break;
        }
    }
    return AABB::FromCenterSize(worldCenter, worldSize);
}
```

### Paso 3: Agregar Validación

```cpp
// src/ECS/Component.h
bool IsValid() const {
    switch (ColliderType) {
        // ... casos existentes ...
        case Type::Pyramid:
            return BaseSize > 0.0f && ApexHeight > 0.0f;
    }
}
```

### ✅ ¡Listo! Pirámides ahora colisionan correctamente.

---

## 🎓 Principios de Diseño

### 1. **AABB Debe Contener Completamente la Forma**
```
✅ Correcto: AABB >= Forma (colisión funciona, puede ser conservadora)
❌ Incorrecto: AABB < Forma (objetos se traspasan)
```

### 2. **Usar Componente de Escala Apropiado**
```cpp
// Esferas: usar scale máximo (no-uniforme)
float maxScale = glm::max(transform->Scale.x, transform->Scale.y, transform->Scale.z);

// Cubos: usar scale directo
glm::vec3 worldSize = localSize * transform->Scale;
```

### 3. **Offset del Centro**
```cpp
// Siempre considerar el offset del centro del collider
glm::vec3 worldCenter = transform->Position + collider->Center * transform->Scale;
```

---

## 🧪 Testing de Nuevas Formas

### Checklist

- [ ] ¿El AABB contiene completamente la forma?
- [ ] ¿Funciona con escalas no-uniformes?
- [ ] ¿Funciona con rotación? (AABB siempre alineado a ejes)
- [ ] ¿IsValid() retorna true para parámetros válidos?
- [ ] ¿Los logs de validación funcionan?

### Test Manual

```cpp
// En SetupDemoScene()
Entity pyramid = m_World->CreateEntity("TestPyramid");
auto* transform = m_World->AddComponent<TransformComponent>(pyramid);
transform->Position = glm::vec3(5.0f, 5.0f, 0.0f);

auto* collider = m_World->AddComponent<ColliderComponent>(pyramid);
collider->ColliderType = ColliderComponent::Type::Pyramid;
collider->BaseSize = 2.0f;
collider->ApexHeight = 3.0f;

auto* rb = m_World->AddComponent<RigidbodyComponent>(pyramid);
rb->UseGravity = true;

m_PhysicsWorld->RegisterRigidbody(rb, collider, transform);

// Ejecutar y verificar:
// 1. Cae con gravedad
// 2. Rebota en el suelo
// 3. NO traspasa otros objetos
```

---

## 🚀 Formas Futuras Sugeridas

### Fase 4+
- **Cylinder**: Similar a Capsule pero sin hemisferios
- **Cone**: Base circular, ápice puntiagudo
- **Torus**: Anillo 3D (major radius + minor radius)
- **ConvexHull**: Envoltura convexa de puntos
- **CompoundShape**: Múltiples colliders en un objeto

### Optimizaciones Avanzadas
- **Narrow-phase**: Para formas precisas (GJK, SAT)
- **Spatial hashing**: Para muchos objetos
- **BVH**: Para meshes complejas

---

## 📊 Rendimiento

### Complejidad

| Operación | Complejidad | Nota |
|-----------|-------------|------|
| AABB vs AABB | O(1) | 6 comparaciones |
| GetWorldAABB | O(1) | Simple cálculo |
| Update (N objetos) | O(N²) | Broad-phase naive |

### Optimización Actual
- ✅ Solo objetos dinámicos vs dinámicos
- ✅ Solo objetos dinámicos vs estáticos
- ✅ Estáticos nunca chequeados entre sí

### Futuras Optimizaciones
- [ ] Spatial hashing: O(N) en promedio
- [ ] Sleeping objects: Skip objetos inmóviles
- [ ] Broad-phase tree (BVH): O(N log N)

---

## 🐛 Debugging

### Logs de Validación
```
[ERROR] Attempting to register INVALID collider!
  Type: 0, Size: (0, 0, 0), Radius: 0, Height: 0
```

**Solución:** Verificar que Size/Radius/Height sean > 0.

### Penetración Visible
```
[SÍNTOMA] Objeto atraviesa el suelo
[CAUSA] AABB no cubre la forma completamente
[FIX] Aumentar Size o Radius en GetWorldAABB()
```

### Colisión Prematura
```
[SÍNTOMA] Objetos colisionan antes de tocarse
[CAUSA] AABB demasiado grande (aceptable)
[FIX] Opcional: usar narrow-phase para precisión
```

---

## 📖 Referencias

### Archivos Clave
- `src/ECS/Component.h` - Definición de ColliderComponent
- `src/Physics/PhysicsWorld.cpp` - Lógica de colisión
- `src/Physics/Collision.h` - Estructuras AABB y Ray

### Documentación Relacionada
- [PHASE3_DEMO.md](PHASE3_DEMO.md) - Ejemplos de uso
- [CHANGELOG.md](CHANGELOG.md) - Historia de cambios

---

## ✅ Garantías del Sistema

1. **✅ No penetración:** Ningún objeto puede traspasar otro
2. **✅ Polimorfismo:** Cualquier forma puede agregarse fácilmente
3. **✅ Validación:** Errores detectados en tiempo de registro
4. **✅ Extensibilidad:** Agregar formas no requiere cambiar algoritmo
5. **✅ Rendimiento:** O(1) por chequeo de colisión

---

**Última Actualización:** Dic 2025  
**Versión del Motor:** Phase 3 - Gameplay Foundations

