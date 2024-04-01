import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk

def rgb_to_rgb565(r, g, b):
    # 将RGB值转换为RGB565格式
    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b

def image_to_rgb565(image):
    try:
        # 转换图像为RGB模式
        img = image.convert("RGB")
        
        # 获取图像的宽度和高度
        width, height = img.size
        
        # 创建一个存储RGB565数据的列表
        rgb565_data = []
        
        # 遍历图像的每个像素，并转换为RGB565格式
        for y in range(height):
            for x in range(width):
                r, g, b = img.getpixel((x, y))
                rgb565_value = rgb_to_rgb565(r, g, b)
                rgb565_data.append(rgb565_value)
                
        return rgb565_data, width, height
        
    except Exception as e:
        print("Error:", e)
        return None

def open_images():
    file_paths = filedialog.askopenfilenames(filetypes=[("JPEG Files", "*.jpg")])
    if file_paths:
        global img_list, img_width_list, img_height_list, img_tk_list, frames
        frames = len(file_paths)
        img_list, img_width_list, img_height_list, img_tk_list = [], [], [], []
        
        for file_path in file_paths:
            img = Image.open(file_path)
            img_width, img_height = img.size
            img_tk = ImageTk.PhotoImage(img)
            img_list.append(img)
            img_width_list.append(img_width)
            img_height_list.append(img_height)
            img_tk_list.append(img_tk)
        
        show_image(0)

def show_image(index):
    image_label.config(image=img_tk_list[index])
    export_button.config(state=tk.NORMAL)

def export_images():
    if img_list:
        name = name_var.get()
        export_path = filedialog.asksaveasfilename(defaultextension=".h")
        if export_path:
            try:
                with open(export_path, 'w') as f:
         
                    f.write(f"int frames ={frames}; \n")
                    f.write(f"const uint16_t PROGMEM {name}[{frames}][{img_width_list[0] * img_height_list[0]}] = {{\n")

                    for i, img in enumerate(img_list):
                        rgb565_data, width, height = image_to_rgb565(img)
                        if rgb565_data:
                            f.write("{")
                            for j, value in enumerate(rgb565_data):
                                f.write("0x{:04X}".format(value))
                                if j < len(rgb565_data) - 1:
                                    f.write(", ")
                                if (j + 1) % width == 0:
                                    f.write("\n")
                            f.write("},\n")
                        else:
                            f.write("// Error converting image to RGB565!\n")

                    f.write("};\n\n")

                    export_label.config(text="Images exported to " + export_path)
            except Exception as e:
                export_label.config(text="Error: " + str(e))

# 创建GUI窗口
window = tk.Tk()
window.title("多张图RGB565转换器by伊娃")
window.geometry("400x500")

# 创建用于显示图像的标签
image_label = tk.Label(window)
image_label.pack()

# 创建输入文字的标签和输入框
name_label = tk.Label(window, text="数组名称:")
name_label.pack()
name_var = tk.StringVar()
name_entry = tk.Entry(window, textvariable=name_var)
name_entry.pack()

# 创建打开按钮
open_button = tk.Button(window, text="开启图片，一次多张", command=open_images)
open_button.pack()

# 创建导出按钮
export_button = tk.Button(window, text="导出h文件", command=export_images, state=tk.DISABLED)
export_button.pack()

# 创建用于显示导出状态的标签
export_label = tk.Label(window, text="")
export_label.pack()

export_label = tk.Label(window, text="使用说明：")
export_label.pack()

export_label = tk.Label(window, text="1.输入数组名称,跟Arduino对应的")
export_label.pack()

export_label = tk.Label(window, text="2.点击开启图片，一次多张，一次选取所有要导入的图片，图片按照顺序")
export_label.pack()

export_label = tk.Label(window, text="3.点击导出h文件")
export_label.pack()

export_label = tk.Label(window, text="比如数组是img，Arduino就要输入tft.pushImage(0,0,128,128,img[i])")
export_label.pack()

img_list, img_width_list, img_height_list, img_tk_list = [], [], [], []
frames = 0

window.mainloop()
