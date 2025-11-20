#!/bin/bash
# 项目编译和测试脚本

echo "==================================="
echo "STC89C52RC 项目编译脚本"
echo "==================================="
echo ""

# 检查 PlatformIO 是否安装
if ! command -v platformio &> /dev/null; then
    echo "❌ PlatformIO 未安装"
    echo "请先安装: pip install platformio"
    exit 1
fi

echo "✅ PlatformIO 已安装"
echo ""

# 清除之前的编译产物
echo "📦 清除之前的编译产物..."
platformio run -e STC89C52RC --target clean

echo ""
echo "🔨 编译项目..."
# 编译项目
if platformio run -e STC89C52RC; then
    echo ""
    echo "✅ 编译成功!"
    echo ""
    echo "编译结果:"
    echo "  - 目标: STC89C52RC"
    echo "  - 平台: intel_mcs51"
    echo "  - 优化: -Os (代码大小优化)"
    echo ""
    echo "📊 下一步操作:"
    echo "  1. 连接 STC89C52RC 开发板"
    echo "  2. 执行烧录命令: platformio run -e STC89C52RC --target upload"
    echo ""
else
    echo ""
    echo "❌ 编译失败"
    echo "请检查错误信息并修复"
    exit 1
fi

echo "✨ 完成!"
