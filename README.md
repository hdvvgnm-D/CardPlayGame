# 纸牌游戏程序说明

## 一、程序结构概览

本项目基于 **Cocos2d-x**，使用 **MVC 架构**组织代码，结构如下：

### 1. models（数据层）

- **CardModel.h/cpp**：描述单张卡牌的属性（花色、点数、位置等）。
- **GameModel.h/cpp**：管理所有卡牌数据（主区、牌堆、手牌）。
- **UndoModel.h/cpp**：记录玩家每一步操作的数据，用于回退。

### 2. views（视图层）

- **CardView.h/cpp**：卡牌的显示、点击交互等。
- **GameView.h/cpp**：游戏主界面，负责卡牌布局、显示、动画等。

### 3. controllers（控制层）

- **GameController.h/cpp**：负责游戏主要逻辑，包括关卡加载、回退等。

### 4. managers

- **UndoManager.h/cpp**：管理撤销（Undo）操作。

### 5. configs/loaders

- **LevelConfigLoader.h/cpp**：读取 JSON 关卡配置并生成卡牌数据。

------

# 二、游戏场景初始化流程

以下为 **GameScene → GameView → 卡牌加载 → 事件绑定 → 挂载到场景** 的完整流程：

1. **创建 GameView 并挂载到场景**
   目标是将 `GameView` 通过 `addChild()` 加到 `GameScene`。
2. **GameView 组成**
   - 背景层
   - 多张卡牌 Sprite
     `GameView` 由 `GameController` 持有。
3. **加载关卡数据**
   调用 `LevelConfigLoader::loadLevelFromFile` 读取 JSON 配置。
4. **初始化 GameView** (`GameView::init`)
   - 创建背景并挂载
   - 自动创建所有 `CardView`（内部调用 `CardView::create`）
   - 初始化 Undo 按钮并加入 GameView
5. **将 GameView 注册到 UndoManager**
   `UndoManager::setGameView()` 用于提供界面引用以执行回退动画。
6. **渲染所有卡牌** (`GameView::showAllCards`)
   - 为主区卡绑定点击事件 `enableMainCardTouch()`
   - 为牌堆卡绑定点击事件 `enablePileCardTouch()`
   - 将牌堆第一张移入手牌区 `moveCardToHandZone()`
   - 牌堆左移 `leftMove()`
   - 将CardView挂载到GameView上
7. **最终挂载到场景**
   `Node::addChild(gameView)`，完成初始化。

------

# 三、游戏运行逻辑：点击移动卡牌到手牌区

## 1. 点击检测

### 主牌区 Main

- 调用 `CardView::isClickable()` 判断是否可点
- 判断是否点中卡牌区域：`Rect::containsPoint()`
- 若可点击且符合游戏规则 → 执行 `moveCardToHandZone()`

### 牌堆区 Pile

- 必须为栈顶元素：

```
if (_pileCards.empty() ||
    cardView->getModel()->getId() != _pileCards.back()->getModel()->getId())
    return false;
```

- 点击区域检测通过后 → `moveCardToHandZone()`

------

# 四、核心逻辑：GameView::moveCardToHandZone

移动动画 + 数据模型更新 + 点击事件解绑与重绑定。

## 1. 整体逻辑结构

```
auto sequence = Sequence::create(moveAction, callFunc, nullptr);
```

包含两部分：

------

## 2. moveAction（移动动画）

- 存储旧位置、旧 zOrder
- 执行 MoveTo 动作移动到手牌区

------

## 3. callFunc（移动结束后执行核心逻辑）

内容包括：

### (1) 将移动信息压入 UndoManager 的历史栈

便于后续撤销。

### (2) 移除当前卡牌的点击监听器

避免移动中点击出错。

### (3) 判断卡牌来源区域：

#### 若来自 **牌堆 pile**

- `GameModel::addPileCardToHand()`（数据层）
- `_pileCards.erase()`（视图层删除）
- 牌堆整体右移：`rightMove()`

#### 若来自 **主区 main**

- `GameModel::addMainCardToHand()`
- `_mainCards.erase()`
- 重新设置主区卡牌的遮挡关系：`GameController::updateCoverRelations()`

### (4) 添加到手牌区

```
_handCards.push_back(cardView);
```

------

# 五、游戏回退（UNDO）逻辑

## 1. 数据记录

玩家每一步操作都会被压入：

```
std::stack<UndoModel*> _history;
```

## 2. 触发回退

点击 Undo 按钮 → 调用：

```
UndoManager::undoLastMove()
```

------

## 3. UndoManager::undoLastMove 核心流程

1. 取出 `_history` 栈顶操作并弹出
2. 若操作来自牌堆 → 执行 `leftMove()` 使牌堆恢复
3. 创建移动动画 `moveAction` 将卡牌移动回原位置
4. 创建 `callFunc`：
   - 恢复卡牌数据到原区域
   - 重新绑定点击事件
5. 执行 `sequence` 完成回退动画与数据恢复

------

# 六、如何新增一张卡牌（关卡扩展）

## 1. 在 JSON 配置文件中添加：

```
{
    "CardFace": 12,
    "CardSuit": 0,
    "Position": {"x": 250, "y": 1000}
}
```

## 2. 加载 JSON 时

`LevelConfigLoader` 会自动创建：

- CardModel
- CardView

并加入 GameModel 和 GameView。

## 3. 在运行时动态添加

调用：

```
GameModel::addMainCard(newCardModel);
```

然后在 GameView 中刷新主区布局。

------

# 七、如何新增一种回退（UNDO）类型

1. 在 `UndoModel` 增加需要记录的新字段
2. 在 `UndoManager::recordCardMove()` 或新方法中记录此类型
3. 在 `UndoManager::undoLastMove()` 中加入对应恢复逻辑
4. 在 `GameController::onUndoMove()` 中同步界面与数据

------

# 八、扩展开发建议

- 所有卡牌操作应通过 GameModel & GameView 的接口进行，保持数据与界面同步
- 每一种可被撤销的动作，都应生成一条 UndoModel 记录
- 统一事件绑定与解绑，避免因动画中事件触发导致错误
- 复杂逻辑集中在 GameController 中统一调度，更易维护
