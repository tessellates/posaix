import numpy as np

def solve_transformation_matrix(src_coords, dst_coords):
    """Solve for the transformation matrix from src_coords to dst_coords."""
    matrix = []
    matrix2 = []
    for src, dst in zip(src_coords, dst_coords):
        matrix.append([src[0], src[1], 1, 0, 0, 0, -dst[0]*src[0], -dst[0]*src[1], -dst[0]])
        matrix.append([0, 0, 0, src[0], src[1], 1, -dst[1]*src[0], -dst[1]*src[1], -dst[1]])
    #matrix2.append([1,1,1,1,1,1,1,1,1,1])
    matrix = np.array(matrix)
    matrix2 = np.transpose(matrix)
    matrix3 = matrix2 @ matrix
    np.set_printoptions(suppress=True)
    np.set_printoptions(precision=3)
    eigenvalues, eigenvectors = np.linalg.eig(matrix3)
    print(eigenvectors)
    print(eigenvalues)
    _, _, V = np.linalg.svd(matrix)
    return V[-1].reshape(3, 3)

def apply_transformation(src_image, transformation_matrix, output_size):
    """Apply a projective transformation to an image without interpolation."""
    inverse_matrix = np.linalg.inv(transformation_matrix)
    dst_image = np.zeros((output_size, output_size, 3), dtype=np.uint8)
    
    for y in range(output_size):
        for x in range(output_size):
            original_pos = np.dot(inverse_matrix, [x, y, 1])
            original_pos /= original_pos[2]
            orig_x, orig_y = int(original_pos[0]), int(original_pos[1])
            
            if 0 <= orig_x < output_size and 0 <= orig_y < output_size:
                dst_image[y, x] = src_image[orig_y, orig_x]
    
    return dst_image

# Generate a square of random colors
size = 35  # Size of the square
square = np.random.randint(0, 256, (size, size, 3), dtype=np.uint8)

# Define the coordinates of the square and the target quadrilateral
square_coords = np.array([[0, 0], [0, size-1], [size-1, size-1], [size-1, 0]], dtype=np.float32)
quad_coords = np.array([[10, 20], [20, size-20], [size-10, size-30], [size-20, 10]], dtype=np.float32)

# Solve for the transformation matrix
transformation_matrix = solve_transformation_matrix(square_coords, quad_coords)
print(transformation_matrix)
# Apply the transformation
transformed = apply_transformation(square, transformation_matrix, size)
'''
# Display the original and transformed images
plt.figure(figsize=(10, 5))

plt.subplot(1, 2, 1)
plt.imshow(square)
plt.title("Original Random Square")
plt.axis('off')

plt.subplot(1, 2, 2)
plt.imshow(transformed)
plt.title("Transformed into Quadrilateral (Pure Python)")
plt.axis('off')

plt.show()'''
