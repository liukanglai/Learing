Vim�UnDo� �c�������֙�C0|g/6iNb{����9�,�   1   ;                        (1, 0), (1, -1), (0, -1), (-1, -1)]                             b���    _�                            ����                                                                                                                                                                                                                                                                                                                                                             b���     �   
      0      9        self.motions = [(-1, 0), (-1, 1), (0, 1), (1, 1),5��    
                    �                     5�_�                       #    ����                                                                                                                                                                                                                                                                                                                                                             b���     �   
      0      9        self.motions = [(-2, 0), (-1, 1), (0, 1), (1, 1),5��    
   #                 �                     5�_�                       &    ����                                                                                                                                                                                                                                                                                                                                                             b���     �   
      0      9        self.motions = [(-2, 0), (-2, 1), (0, 1), (1, 1),5��    
   &                 �                     �    
   .                 �                     5�_�                       3    ����                                                                                                                                                                                                                                                                                                                                                             b���     �   
      0      9        self.motions = [(-2, 0), (-2, 2), (0, 2), (1, 1),5��    
   3                 �                     5�_�                       6    ����                                                                                                                                                                                                                                                                                                                                                             b���     �   
      0      9        self.motions = [(-2, 0), (-2, 2), (0, 2), (2, 1),5��    
   6                 �                     5�_�                            ����                                                                                                                                                                                                                                                                                                                                                             b���    �   0            �   /   1                  return obs�   .   0           �   -   /                      obs.add((40, i))�   ,   .                  for i in range(16):�   +   -                      obs.add((30, i))�   *   ,                  for i in range(15, 30):�   )   +           �   (   *                      obs.add((20, i))�   '   )                  for i in range(15):�   &   (                      obs.add((i, 15))�   %   '                  for i in range(10, 21):�   $   &           �   #   %                      obs.add((x - 1, i))�   "   $                  for i in range(y):�   !   #                      obs.add((0, i))�       "                  for i in range(y):�      !           �                             obs.add((i, y - 1))�                        for i in range(x):�                            obs.add((i, 0))�                        for i in range(x):�                 �                        obs = set()�                        y = self.y_range�                        x = self.x_range�                 �                        """�                !        :return: map of obstacles�                '        Initialize obstacles' positions�                        """�                    def obs_map(self):�                 �                        self.obs = obs�                    def update_obs(self, obs):�                 �                !        self.obs = self.obs_map()�                ;                        (2, 0), (2, -2), (0, -2), (-2, -2)]�   
             9        self.motions = [(-2, 0), (-2, 2), (0, 2), (2, 2),�   	                     self.y_range = 31�      
          /        self.x_range = 51  # size of background�      	              def __init__(self):�                
class Env:�                 �                 �                """�                @author: huiming zhou�                Env 2D�                 """�         0      ;                        (1, 0), (1, -1), (0, -1), (-1, -1)]5��                        �                     �       !                 �                     �       %                 �                     �       .                 �                     �       4                                     �       8                                     �                                                �                                              �                                              �                         !                     �                           %                       �                           &                       �               
       
   '       
       
       �                          2                      �               /          3       /              �    	                  /   K              /       �    
           9          {       9              �               ;       J   �       ;       J       �               !       *   �       !       *       �                       !                 !       �                          -                     �                         .                    �                          M                     �                          d                     �                         e                    �               '          |      '              �               !       '   �      !       '       �                      !   �             !       �                          �                     �                          �                     �                         �                    �                         �                    �                                               �                          %                     �                         &                    �                         A                    �                         ]                    �                          x                     �                           �                     �    !                     �                    �    "                     �                    �    #                     �                    �    $                      �                     �    %                                           �    &                                         �    '                     ,                    �    (                     I                    �    )                      e                     �    *                      �                     �    +                     �                    �    ,                     �                    �    -                     �                    �    .                      �                     �    /                      �                     �    0                      �                     5��