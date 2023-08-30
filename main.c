#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct point
{
    int x;
    int y;
} point_t;

point_t *create_point(point_t *ptr_point, int size);

int main()
{
    point_t *new_points;
    int size;
    
    printf("how many points: ?\n");
    scanf("%d", &size);
    new_points = create_point(new_points, size);

    for (int i = 0; i < size; i++)
    {
        printf("point[%d]-> <%d,%d>\n", i, new_points[i].x,new_points[i].y);
    }

        return 0;
}

point_t *create_point(point_t *ptr_point, int size)
{
        ptr_point = (point_t *)calloc(size, sizeof(point_t));
        if(ptr_point == NULL)
        {
            printf("Failed allocat memory\n");
            return NULL;
        }
        for (int i = 0; i < size; i++)
        {
            printf("Point[%d] : <%d,%d>\n", i, ptr_point[i].x, ptr_point[i].y);
        }
        printf("Enter %d points : ",size);
        for (int i = 0; i < size; i++)
        {
            scanf("%d %d", &ptr_point[i].x, &ptr_point[i].y);
        }
        return ptr_point;
}
