# PA1 Report

> 刘铠铭 2020030014 计14

## 代码逻辑

### 光线投射算法逻辑

> 逻辑实现见 code/src/main.cpp

1. 解析场景；
2. 遍历屏幕空间中的像素 $(x,y)$；
3. 计算当前像素 $(x,y)$ 处相机出射光线 ray，判断 ray 是否和场景有交点；
4. 若有交点，累计来自所有光源地光强影响；若没有交点，设为背景色；
5. 在该像素填充计算的颜色或背景色；
6. 保存图片。

### 透视相机模型

为 `PerspectiveCamera` 添加成员变量 `fx, fy`，在构造函数中通过视场角与所拍照片长宽计算得到。

实现 `Ray generateRay(const Vector2f &point)`，首先在相机空间下计算得到射线方向，之后通过空间变换得到 Global Space 下的光线表示。

### 几何求交

#### 与球求交

$$
t = \dfrac{-b\pm \sqrt{b^2-4ac}}{2a}
$$

#### 与平面求交

$$
t = \dfrac{(p\prime - o)\cdot N}{d \cdot N}, p\prime 为平面上的点
$$

#### 与三角形求交

利用克莱姆法则求解线性方程组，得到 t 与重心坐标，进而判断是否在三角形内：

![](https://raw.githubusercontent.com/KMing-L/Gallery/main//img/20230322195300.png)

## Honor Code

未与任何同学进行代码与思路交流，参考文档内的代码与 Games101 相关课件。