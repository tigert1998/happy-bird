# `GLSL`着色器类

## 接口

```cpp
class Shader {
public:
	Shader() = delete;
	Shader(const std::string &vs_path, const std::string &fs_path);
	void Use() const;
	template <typename T> void SetUniform(const std::string &identifier, const T&) const;

private:
	const FileManager file_manager = FileManager();

	static uint32_t Compile(GLenum type, const std::string &source, const std::string &path);
	static uint32_t Link(uint32_t vs_id, uint32_t fs_id);

	uint32_t id;
};
```

其中默认构造函数被显式删除了，只提供`Shader::Shader(const string &, const string &)`的构造函数，两个字符串分别是定点着色器和像素着色器的地址；`Shader::Use()`只是`glUseProgram(uint32_t)`的封装。

而`template <typename T> Shader::SetUniform<T>(const string &, const T &)`则是其中最重要的接口，它是`C++`程序和运行在`GPU`上的`GLSL`连接点。它提供给`C++`环境直接设置`GLSL`全局变量的能力。其中我们支持的模板参数有`btTransform`、`glm::vec3`、`glm::mat4`、`int32_t`、`float`、`Attenuation`、`Material`、`Light`和`LightCollection`。囊括了我们渲染所需要的全部变量（各种光源、光线衰减、矩阵、坐标空间转换等等）。

# 光线衰减

## 接口

```cpp
class Attenuation {
private:
	float range_;
	float constant_;
	float linear_;
	float quadratic_;
	
public:
	Attenuation(float range);
	float range() const;
	float constant() const;
	float linear() const;
	float quadratic() const;
};
```

`Attenuation`类主要是个数据类，提供便捷的设置光线衰减参数的接口，并可以在`Shader`类中快速设置`GLSL`中对应的光线衰减结构体。

光线衰减一直到渲染时才会真正起作用：

```
float distance = length(light.position - vPosition);
float attenuation = 0.0f;
if (distance < light.attenuation.range) {
	attenuation = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * pow(distance, 2));  
}
	
vec3 diffuseColor = diffuseFactor * material.diffuse * light.color;
vec3 specularColor = specularFactor * material.specular * light.color ;
return attenuation * (diffuseColor + specularColor);
```

上述着色器代码简单的展示了`attenuation`会在渲染中发生的作用，它会根据距离自适应地调整亮度（显然是越远越暗），让渲染效果看起来自然。

# 各类光源

 ## 接口

### 光的纯虚基类

```cpp
enum class LightType {
	kPoint, kParallel, kSpot
};

class Light {
private:
	glm::vec3 color_;
	float intensity_;

public:
	virtual LightType type() const = 0;
	Light(glm::vec3 color, float intensity);
	virtual void Attach(std::weak_ptr<Object>) = 0;
	glm::vec3 color() const;
	float intensity() const;
};
```

### 点光源

```cpp
class PointLight: virtual public Light {
 protected:
	glm::vec3 position_;
	Attenuation attenuation_;
	std::weak_ptr<Object> anchor_;
 public:
	PointLight(glm::vec3 position, glm::vec3 color, float intensity, const Attenuation& attenuation);
	void Attach(std::weak_ptr<Object>);
	LightType type() const; 
	glm::vec3 position() const;
	Attenuation attenuation() const;
};
```
点光源是包含**位置**和**衰减**信息的光。

### 平行光

```cpp
class ParallelLight: virtual public Light {
 protected:
	glm::vec3 direction_;
 public:
	ParallelLight(glm::vec3 direction, glm::vec3 color, float intensity);
	void Attach(std::weak_ptr<Object>);
	LightType type() const; 
	virtual glm::vec3 direction() const;
};
```

平行光是包含**方向**信息的光。

### 聚光灯

```cpp
class SpotLight: public PointLight, public ParallelLight {
 protected:
	float angle_;
 public:
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, const Attenuation& attenuation, float angle);
	void Attach(std::weak_ptr<Object>);
	float angle() const;
	glm::vec3 direction(void) const;
	LightType type() const;
};
```
聚光灯则既是平行光（包含方向信息），也是点光源（包含位置和光线衰减信息）。

### 光线集合

```cpp
class LightCollection {
private:
	glm::vec3 ambient_;
	std::vector<const Light*> light_ptrs_;

public:
	LightCollection(glm::vec3 ambient);
 	void PushBack(const Light* light);
 	const Light* operator[](int i) const;
 	int Size() const;
 	glm::vec3 ambient() const;
};
```

`LightCollection`则是简单地封装了多光源的场景。通过`Shader`类可以直接赋值给`GLSL`中对用的`uniform`变量。

# Controller

## 纯虚基类

```cpp
class Controller {
protected:
	Character &controlee_;

public:
	Controller() = delete;
	Controller(Character &controlee);
	virtual void Elapse(double time) = 0;
};
```

其中`Character`是一个拥有`Move`，`Rotate`或是`Jump`等操作方式的角色（这样的角色可以是人类角色、可以是怪物等等）。而`Controller`的作用就是抽象出它们的运动逻辑并操控这一类`Character`。

另外`Character::Elapse(double)`的作用是刷新角色模拟。因为真正连续的操作是不可能实现的，可以实现的只有在一帧一帧画面之间离散的模拟运动逻辑。它被设计成一个纯虚函数，留给子类单独实现。

## 使用键盘控制的角色控制器

```cpp
class KeyboardController: public Controller {
private:
	Keyboard &keyboard_;
	
public:
	KeyboardController() = delete;
	KeyboardController(Character &controlee, Keyboard &keyboard = Keyboard::shared);
	void Elapse(double time);
};
```

`KeyboardController`绑定一个`Character`和一个键盘实例，每当键盘触发一些事件的时候，都会在`Keyboard::Elapse(double)`中以回调的形式发送给`KeyboardController`，之后用这个刷新事件，并控制主角的移动。

## 自动机的`NPC`控制器

```cpp
class AutomationController: public Controller {
private:
	enum class PatrolDirection {
		kXPosition, kXNegative
	};
	PatrolDirection patrol_direction_;
	Character &target_;
	std::shared_ptr<glm::vec3> patrol_ptr_;
	float patrol_radius_;

public:
	AutomationController() = delete;
	AutomationController(Character &controlee, Character &target, float patrol_radius = 10);
	void Elapse(double time);
};
```

自动机控制器不依赖于键盘或是鼠标等外设，它会绑定一个被控制者`controlee`，它的目标角色（即攻击对象）`target`以及一个巡逻半径。当角色在视角之外的时候，怪物就会按照特定的巡逻半径周期性巡逻，否则就会追着攻击对象跑。