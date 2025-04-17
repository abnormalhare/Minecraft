==== com/mojang/rubydung/character/Cube.java ====
package com.mojang.rubydung.character;

import org.lwjgl.opengl.GL11;

public class Cube {
   private Vertex[] vertices;
   private Polygon[] polygons;
   private int xTexOffs;
   private int yTexOffs;
   public float x;
   public float y;
   public float z;
   public float xRot;
   public float yRot;
   public float zRot;
   private boolean compiled = false;
   private int list = 0;

   public Cube(int xTexOffs, int yTexOffs) {
      this.xTexOffs = xTexOffs;
      this.yTexOffs = yTexOffs;
   }

   public void setTexOffs(int xTexOffs, int yTexOffs) {
      this.xTexOffs = xTexOffs;
      this.yTexOffs = yTexOffs;
   }

   public void addBox(float x0, float y0, float z0, int w, int h, int d) {
      this.vertices = new Vertex[8];
      this.polygons = new Polygon[6];
      float x1 = x0 + w;
      float y1 = y0 + h;
      float z1 = z0 + d;
      Vertex u0 = new Vertex(x0, y0, z0, 0.0F, 0.0F);
      Vertex u1 = new Vertex(x1, y0, z0, 0.0F, 8.0F);
      Vertex u2 = new Vertex(x1, y1, z0, 8.0F, 8.0F);
      Vertex u3 = new Vertex(x0, y1, z0, 8.0F, 0.0F);
      Vertex l0 = new Vertex(x0, y0, z1, 0.0F, 0.0F);
      Vertex l1 = new Vertex(x1, y0, z1, 0.0F, 8.0F);
      Vertex l2 = new Vertex(x1, y1, z1, 8.0F, 8.0F);
      Vertex l3 = new Vertex(x0, y1, z1, 8.0F, 0.0F);
      this.vertices[0] = u0;
      this.vertices[1] = u1;
      this.vertices[2] = u2;
      this.vertices[3] = u3;
      this.vertices[4] = l0;
      this.vertices[5] = l1;
      this.vertices[6] = l2;
      this.vertices[7] = l3;
      this.polygons[0] = new Polygon(new Vertex[]{l1, u1, u2, l2}, this.xTexOffs + d + w, this.yTexOffs + d, this.xTexOffs + d + w + d, this.yTexOffs + d + h);
      this.polygons[1] = new Polygon(new Vertex[]{u0, l0, l3, u3}, this.xTexOffs + 0, this.yTexOffs + d, this.xTexOffs + d, this.yTexOffs + d + h);
      this.polygons[2] = new Polygon(new Vertex[]{l1, l0, u0, u1}, this.xTexOffs + d, this.yTexOffs + 0, this.xTexOffs + d + w, this.yTexOffs + d);
      this.polygons[3] = new Polygon(new Vertex[]{u2, u3, l3, l2}, this.xTexOffs + d + w, this.yTexOffs + 0, this.xTexOffs + d + w + w, this.yTexOffs + d);
      this.polygons[4] = new Polygon(new Vertex[]{u1, u0, u3, u2}, this.xTexOffs + d, this.yTexOffs + d, this.xTexOffs + d + w, this.yTexOffs + d + h);
      this.polygons[5] = new Polygon(
         new Vertex[]{l0, l1, l2, l3}, this.xTexOffs + d + w + d, this.yTexOffs + d, this.xTexOffs + d + w + d + w, this.yTexOffs + d + h
      );
   }

   public void setPos(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
   }

   public void render() {
      if (!this.compiled) {
         this.compile();
      }

      float c = (float) (180.0 / Math.PI);
      GL11.glPushMatrix();
      GL11.glTranslatef(this.x, this.y, this.z);
      GL11.glRotatef(this.zRot * c, 0.0F, 0.0F, 1.0F);
      GL11.glRotatef(this.yRot * c, 0.0F, 1.0F, 0.0F);
      GL11.glRotatef(this.xRot * c, 1.0F, 0.0F, 0.0F);
      GL11.glCallList(this.list);
      GL11.glPopMatrix();
   }

   private void compile() {
      this.list = GL11.glGenLists(1);
      GL11.glNewList(this.list, 4864);
      GL11.glBegin(7);

      for (int i = 0; i < this.polygons.length; i++) {
         this.polygons[i].render();
      }

      GL11.glEnd();
      GL11.glEndList();
      this.compiled = true;
   }
}

==== com/mojang/rubydung/character/Polygon.java ====
package com.mojang.rubydung.character;

import org.lwjgl.opengl.GL11;

public class Polygon {
   public Vertex[] vertices;
   public int vertexCount = 0;

   public Polygon(Vertex[] vertices) {
      this.vertices = vertices;
      this.vertexCount = vertices.length;
   }

   public Polygon(Vertex[] vertices, int u0, int v0, int u1, int v1) {
      this(vertices);
      vertices[0] = vertices[0].remap(u1, v0);
      vertices[1] = vertices[1].remap(u0, v0);
      vertices[2] = vertices[2].remap(u0, v1);
      vertices[3] = vertices[3].remap(u1, v1);
   }

   public void render() {
      GL11.glColor3f(1.0F, 1.0F, 1.0F);

      for (int i = 3; i >= 0; i--) {
         Vertex v = this.vertices[i];
         GL11.glTexCoord2f(v.u / 63.999F, v.v / 31.999F);
         GL11.glVertex3f(v.pos.x, v.pos.y, v.pos.z);
      }
   }
}

==== com/mojang/rubydung/character/Vec3.java ====
package com.mojang.rubydung.character;

public class Vec3 {
   public float x;
   public float y;
   public float z;

   public Vec3(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
   }

   public Vec3 interpolateTo(Vec3 t, float p) {
      float xt = this.x + (t.x - this.x) * p;
      float yt = this.y + (t.y - this.y) * p;
      float zt = this.z + (t.z - this.z) * p;
      return new Vec3(xt, yt, zt);
   }

   public void set(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
   }
}

==== com/mojang/rubydung/character/Vertex.java ====
package com.mojang.rubydung.character;

public class Vertex {
   public Vec3 pos;
   public float u;
   public float v;

   public Vertex(float x, float y, float z, float u, float v) {
      this(new Vec3(x, y, z), u, v);
   }

   public Vertex remap(float u, float v) {
      return new Vertex(this, u, v);
   }

   public Vertex(Vertex vertex, float u, float v) {
      this.pos = vertex.pos;
      this.u = u;
      this.v = v;
   }

   public Vertex(Vec3 pos, float u, float v) {
      this.pos = pos;
      this.u = u;
      this.v = v;
   }
}

==== com/mojang/rubydung/character/Zombie.java ====
package com.mojang.rubydung.character;

import com.mojang.rubydung.Entity;
import com.mojang.rubydung.Textures;
import com.mojang.rubydung.level.Level;
import org.lwjgl.opengl.GL11;

public class Zombie extends Entity {
   public float rot;
   public float timeOffs;
   public float speed;
   public float rotA = (float)(Math.random() + 1.0) * 0.01F;
   private static ZombieModel zombieModel = new ZombieModel();

   public Zombie(Level level, float x, float y, float z) {
      super(level);
      this.setPos(x, y, z);
      this.timeOffs = (float)Math.random() * 1239813.0F;
      this.rot = (float)(Math.random() * Math.PI * 2.0);
      this.speed = 1.0F;
   }

   @Override
   public void tick() {
      this.xo = this.x;
      this.yo = this.y;
      this.zo = this.z;
      float xa = 0.0F;
      float ya = 0.0F;
      if (this.y < -100.0F) {
         this.remove();
      }

      this.rot = this.rot + this.rotA;
      this.rotA = (float)(this.rotA * 0.99);
      this.rotA = (float)(this.rotA + (Math.random() - Math.random()) * Math.random() * Math.random() * 0.08F);
      xa = (float)Math.sin(this.rot);
      ya = (float)Math.cos(this.rot);
      if (this.onGround && Math.random() < 0.08) {
         this.yd = 0.5F;
      }

      this.moveRelative(xa, ya, this.onGround ? 0.1F : 0.02F);
      this.yd = (float)(this.yd - 0.08);
      this.move(this.xd, this.yd, this.zd);
      this.xd *= 0.91F;
      this.yd *= 0.98F;
      this.zd *= 0.91F;
      if (this.onGround) {
         this.xd *= 0.7F;
         this.zd *= 0.7F;
      }
   }

   public void render(float a) {
      GL11.glEnable(3553);
      GL11.glBindTexture(3553, Textures.loadTexture("/char.png", 9728));
      GL11.glPushMatrix();
      double time = System.nanoTime() / 1.0E9 * 10.0 * this.speed + this.timeOffs;
      float size = 0.058333334F;
      float yy = (float)(-Math.abs(Math.sin(time * 0.6662)) * 5.0 - 23.0);
      GL11.glTranslatef(this.xo + (this.x - this.xo) * a, this.yo + (this.y - this.yo) * a, this.zo + (this.z - this.zo) * a);
      GL11.glScalef(1.0F, -1.0F, 1.0F);
      GL11.glScalef(size, size, size);
      GL11.glTranslatef(0.0F, yy, 0.0F);
      float c = (float) (180.0 / Math.PI);
      GL11.glRotatef(this.rot * c + 180.0F, 0.0F, 1.0F, 0.0F);
      zombieModel.render((float)time);
      GL11.glPopMatrix();
      GL11.glDisable(3553);
   }
}

==== com/mojang/rubydung/character/ZombieModel.java ====
package com.mojang.rubydung.character;

public class ZombieModel {
   public Cube head = new Cube(0, 0);
   public Cube body;
   public Cube arm0;
   public Cube arm1;
   public Cube leg0;
   public Cube leg1;

   public ZombieModel() {
      this.head.addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8);
      this.body = new Cube(16, 16);
      this.body.addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4);
      this.arm0 = new Cube(40, 16);
      this.arm0.addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4);
      this.arm0.setPos(-5.0F, 2.0F, 0.0F);
      this.arm1 = new Cube(40, 16);
      this.arm1.addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4);
      this.arm1.setPos(5.0F, 2.0F, 0.0F);
      this.leg0 = new Cube(0, 16);
      this.leg0.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
      this.leg0.setPos(-2.0F, 12.0F, 0.0F);
      this.leg1 = new Cube(0, 16);
      this.leg1.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
      this.leg1.setPos(2.0F, 12.0F, 0.0F);
   }

   public void render(float time) {
      this.head.yRot = (float)Math.sin(time * 0.83) * 1.0F;
      this.head.xRot = (float)Math.sin(time) * 0.8F;
      this.arm0.xRot = (float)Math.sin(time * 0.6662 + Math.PI) * 2.0F;
      this.arm0.zRot = (float)(Math.sin(time * 0.2312) + 1.0) * 1.0F;
      this.arm1.xRot = (float)Math.sin(time * 0.6662) * 2.0F;
      this.arm1.zRot = (float)(Math.sin(time * 0.2812) - 1.0) * 1.0F;
      this.leg0.xRot = (float)Math.sin(time * 0.6662) * 1.4F;
      this.leg1.xRot = (float)Math.sin(time * 0.6662 + Math.PI) * 1.4F;
      this.head.render();
      this.body.render();
      this.arm0.render();
      this.arm1.render();
      this.leg0.render();
      this.leg1.render();
   }
}

==== com/mojang/rubydung/Entity.java ====
package com.mojang.rubydung;

import com.mojang.rubydung.level.Level;
import com.mojang.rubydung.phys.AABB;
import java.util.List;

public class Entity {
   protected Level level;
   public float xo;
   public float yo;
   public float zo;
   public float x;
   public float y;
   public float z;
   public float xd;
   public float yd;
   public float zd;
   public float yRot;
   public float xRot;
   public AABB bb;
   public boolean onGround = false;
   public boolean removed = false;
   protected float heightOffset = 0.0F;
   protected float bbWidth = 0.6F;
   protected float bbHeight = 1.8F;

   public Entity(Level level) {
      this.level = level;
      this.resetPos();
   }

   protected void resetPos() {
      float x = (float)Math.random() * this.level.width;
      float y = this.level.depth + 10;
      float z = (float)Math.random() * this.level.height;
      this.setPos(x, y, z);
   }

   public void remove() {
      this.removed = true;
   }

   protected void setSize(float w, float h) {
      this.bbWidth = w;
      this.bbHeight = h;
   }

   protected void setPos(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
      float w = this.bbWidth / 2.0F;
      float h = this.bbHeight / 2.0F;
      this.bb = new AABB(x - w, y - h, z - w, x + w, y + h, z + w);
   }

   public void turn(float xo, float yo) {
      this.yRot = (float)(this.yRot + xo * 0.15);
      this.xRot = (float)(this.xRot - yo * 0.15);
      if (this.xRot < -90.0F) {
         this.xRot = -90.0F;
      }

      if (this.xRot > 90.0F) {
         this.xRot = 90.0F;
      }
   }

   public void tick() {
      this.xo = this.x;
      this.yo = this.y;
      this.zo = this.z;
   }

   public void move(float xa, float ya, float za) {
      float xaOrg = xa;
      float yaOrg = ya;
      float zaOrg = za;
      List<AABB> aABBs = this.level.getCubes(this.bb.expand(xa, ya, za));

      for (int i = 0; i < aABBs.size(); i++) {
         ya = aABBs.get(i).clipYCollide(this.bb, ya);
      }

      this.bb.move(0.0F, ya, 0.0F);

      for (int i = 0; i < aABBs.size(); i++) {
         xa = aABBs.get(i).clipXCollide(this.bb, xa);
      }

      this.bb.move(xa, 0.0F, 0.0F);

      for (int i = 0; i < aABBs.size(); i++) {
         za = aABBs.get(i).clipZCollide(this.bb, za);
      }

      this.bb.move(0.0F, 0.0F, za);
      this.onGround = yaOrg != ya && yaOrg < 0.0F;
      if (xaOrg != xa) {
         this.xd = 0.0F;
      }

      if (yaOrg != ya) {
         this.yd = 0.0F;
      }

      if (zaOrg != za) {
         this.zd = 0.0F;
      }

      this.x = (this.bb.x0 + this.bb.x1) / 2.0F;
      this.y = this.bb.y0 + this.heightOffset;
      this.z = (this.bb.z0 + this.bb.z1) / 2.0F;
   }

   public void moveRelative(float xa, float za, float speed) {
      float dist = xa * xa + za * za;
      if (!(dist < 0.01F)) {
         dist = speed / (float)Math.sqrt(dist);
         xa *= dist;
         za *= dist;
         float sin = (float)Math.sin(this.yRot * Math.PI / 180.0);
         float cos = (float)Math.cos(this.yRot * Math.PI / 180.0);
         this.xd += xa * cos - za * sin;
         this.zd += za * cos + xa * sin;
      }
   }

   public boolean isLit() {
      int xTile = (int)this.x;
      int yTile = (int)this.y;
      int zTile = (int)this.z;
      return this.level.isLit(xTile, yTile, zTile);
   }
}

==== com/mojang/rubydung/HitResult.java ====
package com.mojang.rubydung;

public class HitResult {
   public int x;
   public int y;
   public int z;
   public int o;
   public int f;

   public HitResult(int x, int y, int z, int o, int f) {
      this.x = x;
      this.y = y;
      this.z = z;
      this.o = o;
      this.f = f;
   }
}

==== com/mojang/rubydung/level/Chunk.java ====
package com.mojang.rubydung.level;

import com.mojang.rubydung.Player;
import com.mojang.rubydung.level.tile.Tile;
import com.mojang.rubydung.phys.AABB;
import org.lwjgl.opengl.GL11;

public class Chunk {
   public AABB aabb;
   public final Level level;
   public final int x0;
   public final int y0;
   public final int z0;
   public final int x1;
   public final int y1;
   public final int z1;
   public final float x;
   public final float y;
   public final float z;
   private boolean dirty = true;
   private int lists = -1;
   public long dirtiedTime = 0L;
   private static Tesselator t = Tesselator.instance;
   public static int updates = 0;
   private static long totalTime = 0L;
   private static int totalUpdates = 0;

   public Chunk(Level level, int x0, int y0, int z0, int x1, int y1, int z1) {
      this.level = level;
      this.x0 = x0;
      this.y0 = y0;
      this.z0 = z0;
      this.x1 = x1;
      this.y1 = y1;
      this.z1 = z1;
      this.x = (x0 + x1) / 2.0F;
      this.y = (y0 + y1) / 2.0F;
      this.z = (z0 + z1) / 2.0F;
      this.aabb = new AABB(x0, y0, z0, x1, y1, z1);
      this.lists = GL11.glGenLists(2);
   }

   private void rebuild(int layer) {
      this.dirty = false;
      updates++;
      long before = System.nanoTime();
      GL11.glNewList(this.lists + layer, 4864);
      t.init();
      int tiles = 0;

      for (int x = this.x0; x < this.x1; x++) {
         for (int y = this.y0; y < this.y1; y++) {
            for (int z = this.z0; z < this.z1; z++) {
               int tileId = this.level.getTile(x, y, z);
               if (tileId > 0) {
                  Tile.tiles[tileId].render(t, this.level, layer, x, y, z);
                  tiles++;
               }
            }
         }
      }

      t.flush();
      GL11.glEndList();
      long after = System.nanoTime();
      if (tiles > 0) {
         totalTime += after - before;
         totalUpdates++;
      }
   }

   public void rebuild() {
      this.rebuild(0);
      this.rebuild(1);
   }

   public void render(int layer) {
      GL11.glCallList(this.lists + layer);
   }

   public void setDirty() {
      if (!this.dirty) {
         this.dirtiedTime = System.currentTimeMillis();
      }

      this.dirty = true;
   }

   public boolean isDirty() {
      return this.dirty;
   }

   public float distanceToSqr(Player player) {
      float xd = player.x - this.x;
      float yd = player.y - this.y;
      float zd = player.z - this.z;
      return xd * xd + yd * yd + zd * zd;
   }
}

==== com/mojang/rubydung/level/DirtyChunkSorter.java ====
package com.mojang.rubydung.level;

import com.mojang.rubydung.Player;
import java.util.Comparator;

public class DirtyChunkSorter implements Comparator<Chunk> {
   private Player player;
   private Frustum frustum;
   private long now = System.currentTimeMillis();

   public DirtyChunkSorter(Player player, Frustum frustum) {
      this.player = player;
      this.frustum = frustum;
   }

   public int compare(Chunk c0, Chunk c1) {
      boolean i0 = this.frustum.isVisible(c0.aabb);
      boolean i1 = this.frustum.isVisible(c1.aabb);
      if (i0 && !i1) {
         return -1;
      } else if (i1 && !i0) {
         return 1;
      } else {
         int t0 = (int)((this.now - c0.dirtiedTime) / 2000L);
         int t1 = (int)((this.now - c1.dirtiedTime) / 2000L);
         if (t0 < t1) {
            return -1;
         } else if (t0 > t1) {
            return 1;
         } else {
            return c0.distanceToSqr(this.player) < c1.distanceToSqr(this.player) ? -1 : 1;
         }
      }
   }
}

==== com/mojang/rubydung/level/Frustum.java ====
package com.mojang.rubydung.level;

import com.mojang.rubydung.phys.AABB;
import java.nio.Buffer;
import java.nio.FloatBuffer;
import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;

public class Frustum {
   public float[][] m_Frustum = new float[6][4];
   public static final int RIGHT = 0;
   public static final int LEFT = 1;
   public static final int BOTTOM = 2;
   public static final int TOP = 3;
   public static final int BACK = 4;
   public static final int FRONT = 5;
   public static final int A = 0;
   public static final int B = 1;
   public static final int C = 2;
   public static final int D = 3;
   private static Frustum frustum = new Frustum();
   private FloatBuffer _proj = BufferUtils.createFloatBuffer(16);
   private FloatBuffer _modl = BufferUtils.createFloatBuffer(16);
   private FloatBuffer _clip = BufferUtils.createFloatBuffer(16);
   float[] proj = new float[16];
   float[] modl = new float[16];
   float[] clip = new float[16];

   private Frustum() {
   }

   public static Frustum getFrustum() {
      frustum.calculateFrustum();
      return frustum;
   }

   private void normalizePlane(float[][] frustum, int side) {
      float magnitude = (float)Math.sqrt(frustum[side][0] * frustum[side][0] + frustum[side][1] * frustum[side][1] + frustum[side][2] * frustum[side][2]);
      frustum[side][0] = frustum[side][0] / magnitude;
      frustum[side][1] = frustum[side][1] / magnitude;
      frustum[side][2] = frustum[side][2] / magnitude;
      frustum[side][3] = frustum[side][3] / magnitude;
   }

   private void calculateFrustum() {
      ((Buffer)this._proj).clear();
      ((Buffer)this._modl).clear();
      ((Buffer)this._clip).clear();
      GL11.glGetFloat(2983, this._proj);
      GL11.glGetFloat(2982, this._modl);
      ((Buffer)this._proj).flip().limit(16);
      this._proj.get(this.proj);
      ((Buffer)this._modl).flip().limit(16);
      this._modl.get(this.modl);
      this.clip[0] = this.modl[0] * this.proj[0] + this.modl[1] * this.proj[4] + this.modl[2] * this.proj[8] + this.modl[3] * this.proj[12];
      this.clip[1] = this.modl[0] * this.proj[1] + this.modl[1] * this.proj[5] + this.modl[2] * this.proj[9] + this.modl[3] * this.proj[13];
      this.clip[2] = this.modl[0] * this.proj[2] + this.modl[1] * this.proj[6] + this.modl[2] * this.proj[10] + this.modl[3] * this.proj[14];
      this.clip[3] = this.modl[0] * this.proj[3] + this.modl[1] * this.proj[7] + this.modl[2] * this.proj[11] + this.modl[3] * this.proj[15];
      this.clip[4] = this.modl[4] * this.proj[0] + this.modl[5] * this.proj[4] + this.modl[6] * this.proj[8] + this.modl[7] * this.proj[12];
      this.clip[5] = this.modl[4] * this.proj[1] + this.modl[5] * this.proj[5] + this.modl[6] * this.proj[9] + this.modl[7] * this.proj[13];
      this.clip[6] = this.modl[4] * this.proj[2] + this.modl[5] * this.proj[6] + this.modl[6] * this.proj[10] + this.modl[7] * this.proj[14];
      this.clip[7] = this.modl[4] * this.proj[3] + this.modl[5] * this.proj[7] + this.modl[6] * this.proj[11] + this.modl[7] * this.proj[15];
      this.clip[8] = this.modl[8] * this.proj[0] + this.modl[9] * this.proj[4] + this.modl[10] * this.proj[8] + this.modl[11] * this.proj[12];
      this.clip[9] = this.modl[8] * this.proj[1] + this.modl[9] * this.proj[5] + this.modl[10] * this.proj[9] + this.modl[11] * this.proj[13];
      this.clip[10] = this.modl[8] * this.proj[2] + this.modl[9] * this.proj[6] + this.modl[10] * this.proj[10] + this.modl[11] * this.proj[14];
      this.clip[11] = this.modl[8] * this.proj[3] + this.modl[9] * this.proj[7] + this.modl[10] * this.proj[11] + this.modl[11] * this.proj[15];
      this.clip[12] = this.modl[12] * this.proj[0] + this.modl[13] * this.proj[4] + this.modl[14] * this.proj[8] + this.modl[15] * this.proj[12];
      this.clip[13] = this.modl[12] * this.proj[1] + this.modl[13] * this.proj[5] + this.modl[14] * this.proj[9] + this.modl[15] * this.proj[13];
      this.clip[14] = this.modl[12] * this.proj[2] + this.modl[13] * this.proj[6] + this.modl[14] * this.proj[10] + this.modl[15] * this.proj[14];
      this.clip[15] = this.modl[12] * this.proj[3] + this.modl[13] * this.proj[7] + this.modl[14] * this.proj[11] + this.modl[15] * this.proj[15];
      this.m_Frustum[0][0] = this.clip[3] - this.clip[0];
      this.m_Frustum[0][1] = this.clip[7] - this.clip[4];
      this.m_Frustum[0][2] = this.clip[11] - this.clip[8];
      this.m_Frustum[0][3] = this.clip[15] - this.clip[12];
      this.normalizePlane(this.m_Frustum, 0);
      this.m_Frustum[1][0] = this.clip[3] + this.clip[0];
      this.m_Frustum[1][1] = this.clip[7] + this.clip[4];
      this.m_Frustum[1][2] = this.clip[11] + this.clip[8];
      this.m_Frustum[1][3] = this.clip[15] + this.clip[12];
      this.normalizePlane(this.m_Frustum, 1);
      this.m_Frustum[2][0] = this.clip[3] + this.clip[1];
      this.m_Frustum[2][1] = this.clip[7] + this.clip[5];
      this.m_Frustum[2][2] = this.clip[11] + this.clip[9];
      this.m_Frustum[2][3] = this.clip[15] + this.clip[13];
      this.normalizePlane(this.m_Frustum, 2);
      this.m_Frustum[3][0] = this.clip[3] - this.clip[1];
      this.m_Frustum[3][1] = this.clip[7] - this.clip[5];
      this.m_Frustum[3][2] = this.clip[11] - this.clip[9];
      this.m_Frustum[3][3] = this.clip[15] - this.clip[13];
      this.normalizePlane(this.m_Frustum, 3);
      this.m_Frustum[4][0] = this.clip[3] - this.clip[2];
      this.m_Frustum[4][1] = this.clip[7] - this.clip[6];
      this.m_Frustum[4][2] = this.clip[11] - this.clip[10];
      this.m_Frustum[4][3] = this.clip[15] - this.clip[14];
      this.normalizePlane(this.m_Frustum, 4);
      this.m_Frustum[5][0] = this.clip[3] + this.clip[2];
      this.m_Frustum[5][1] = this.clip[7] + this.clip[6];
      this.m_Frustum[5][2] = this.clip[11] + this.clip[10];
      this.m_Frustum[5][3] = this.clip[15] + this.clip[14];
      this.normalizePlane(this.m_Frustum, 5);
   }

   public boolean pointInFrustum(float x, float y, float z) {
      for (int i = 0; i < 6; i++) {
         if (this.m_Frustum[i][0] * x + this.m_Frustum[i][1] * y + this.m_Frustum[i][2] * z + this.m_Frustum[i][3] <= 0.0F) {
            return false;
         }
      }

      return true;
   }

   public boolean sphereInFrustum(float x, float y, float z, float radius) {
      for (int i = 0; i < 6; i++) {
         if (this.m_Frustum[i][0] * x + this.m_Frustum[i][1] * y + this.m_Frustum[i][2] * z + this.m_Frustum[i][3] <= -radius) {
            return false;
         }
      }

      return true;
   }

   public boolean cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
      for (int i = 0; i < 6; i++) {
         if (!(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }

         if (!(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }
      }

      return true;
   }

   public boolean cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
      for (int i = 0; i < 6; i++) {
         if (!(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z1 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y1 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x1 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)
            && !(this.m_Frustum[i][0] * x2 + this.m_Frustum[i][1] * y2 + this.m_Frustum[i][2] * z2 + this.m_Frustum[i][3] > 0.0F)) {
            return false;
         }
      }

      return true;
   }

   public boolean isVisible(AABB aabb) {
      return this.cubeInFrustum(aabb.x0, aabb.y0, aabb.z0, aabb.x1, aabb.y1, aabb.z1);
   }
}

==== com/mojang/rubydung/level/Level.java ====
package com.mojang.rubydung.level;

import com.mojang.rubydung.level.tile.Tile;
import com.mojang.rubydung.phys.AABB;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.Random;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

public class Level {
   private static final int TILE_UPDATE_INTERVAL = 400;
   public final int width;
   public final int height;
   public final int depth;
   private byte[] blocks;
   private int[] lightDepths;
   private ArrayList<LevelListener> levelListeners = new ArrayList<>();
   private Random random = new Random();
   int unprocessed = 0;

   public Level(int w, int h, int d) {
      this.width = w;
      this.height = h;
      this.depth = d;
      this.blocks = new byte[w * h * d];
      this.lightDepths = new int[w * h];
      boolean mapLoaded = this.load();
      if (!mapLoaded) {
         this.generateMap();
      }

      this.calcLightDepths(0, 0, w, h);
   }

   private void generateMap() {
      int w = this.width;
      int h = this.height;
      int d = this.depth;
      int[] heightmap1 = new PerlinNoiseFilter(0).read(w, h);
      int[] heightmap2 = new PerlinNoiseFilter(0).read(w, h);
      int[] cf = new PerlinNoiseFilter(1).read(w, h);
      int[] rockMap = new PerlinNoiseFilter(1).read(w, h);

      for (int x = 0; x < w; x++) {
         for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
               int dh1 = heightmap1[x + z * this.width];
               int dh2 = heightmap2[x + z * this.width];
               int cfh = cf[x + z * this.width];
               if (cfh < 128) {
                  dh2 = dh1;
               }

               int dh = dh1;
               if (dh2 > dh1) {
                  dh = dh2;
               }

               dh = dh / 8 + d / 3;
               int rh = rockMap[x + z * this.width] / 8 + d / 3;
               if (rh > dh - 2) {
                  rh = dh - 2;
               }

               int i = (y * this.height + z) * this.width + x;
               int id = 0;
               if (y == dh) {
                  id = Tile.grass.id;
               }

               if (y < dh) {
                  id = Tile.dirt.id;
               }

               if (y <= rh) {
                  id = Tile.rock.id;
               }

               this.blocks[i] = (byte)id;
            }
         }
      }
   }

   public boolean load() {
      try {
         DataInputStream dis = new DataInputStream(new GZIPInputStream(new FileInputStream(new File("level.dat"))));
         dis.readFully(this.blocks);
         this.calcLightDepths(0, 0, this.width, this.height);

         for (int i = 0; i < this.levelListeners.size(); i++) {
            this.levelListeners.get(i).allChanged();
         }

         dis.close();
         return true;
      } catch (Exception var3) {
         var3.printStackTrace();
         return false;
      }
   }

   public void save() {
      try {
         DataOutputStream dos = new DataOutputStream(new GZIPOutputStream(new FileOutputStream(new File("level.dat"))));
         dos.write(this.blocks);
         dos.close();
      } catch (Exception var2) {
         var2.printStackTrace();
      }
   }

   public void calcLightDepths(int x0, int y0, int x1, int y1) {
      for (int x = x0; x < x0 + x1; x++) {
         for (int z = y0; z < y0 + y1; z++) {
            int oldDepth = this.lightDepths[x + z * this.width];
            int y = this.depth - 1;

            while (y > 0 && !this.isLightBlocker(x, y, z)) {
               y--;
            }

            this.lightDepths[x + z * this.width] = y;
            if (oldDepth != y) {
               int yl0 = oldDepth < y ? oldDepth : y;
               int yl1 = oldDepth > y ? oldDepth : y;

               for (int i = 0; i < this.levelListeners.size(); i++) {
                  this.levelListeners.get(i).lightColumnChanged(x, z, yl0, yl1);
               }
            }
         }
      }
   }

   public void addListener(LevelListener levelListener) {
      this.levelListeners.add(levelListener);
   }

   public void removeListener(LevelListener levelListener) {
      this.levelListeners.remove(levelListener);
   }

   public boolean isLightBlocker(int x, int y, int z) {
      Tile tile = Tile.tiles[this.getTile(x, y, z)];
      return tile == null ? false : tile.blocksLight();
   }

   public ArrayList<AABB> getCubes(AABB aABB) {
      ArrayList<AABB> aABBs = new ArrayList<>();
      int x0 = (int)aABB.x0;
      int x1 = (int)(aABB.x1 + 1.0F);
      int y0 = (int)aABB.y0;
      int y1 = (int)(aABB.y1 + 1.0F);
      int z0 = (int)aABB.z0;
      int z1 = (int)(aABB.z1 + 1.0F);
      if (x0 < 0) {
         x0 = 0;
      }

      if (y0 < 0) {
         y0 = 0;
      }

      if (z0 < 0) {
         z0 = 0;
      }

      if (x1 > this.width) {
         x1 = this.width;
      }

      if (y1 > this.depth) {
         y1 = this.depth;
      }

      if (z1 > this.height) {
         z1 = this.height;
      }

      for (int x = x0; x < x1; x++) {
         for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
               Tile tile = Tile.tiles[this.getTile(x, y, z)];
               if (tile != null) {
                  aABBs.add(tile.getAABB(x, y, z));
               }
            }
         }
      }

      return aABBs;
   }

   public boolean setTile(int x, int y, int z, int type) {
      if (x >= 0 && y >= 0 && z >= 0 && x < this.width && y < this.depth && z < this.height) {
         if (type == this.blocks[(y * this.height + z) * this.width + x]) {
            return false;
         } else {
            this.blocks[(y * this.height + z) * this.width + x] = (byte)type;
            this.calcLightDepths(x, z, 1, 1);

            for (int i = 0; i < this.levelListeners.size(); i++) {
               this.levelListeners.get(i).tileChanged(x, y, z);
            }

            return true;
         }
      } else {
         return false;
      }
   }

   public boolean isLit(int x, int y, int z) {
      return x < 0 || y < 0 || z < 0 || x >= this.width || y >= this.depth || z >= this.height ? true : y >= this.lightDepths[x + z * this.width];
   }

   public int getTile(int x, int y, int z) {
      return x >= 0 && y >= 0 && z >= 0 && x < this.width && y < this.depth && z < this.height ? this.blocks[(y * this.height + z) * this.width + x] : 0;
   }

   public boolean isSolidTile(int x, int y, int z) {
      Tile tile = Tile.tiles[this.getTile(x, y, z)];
      return tile == null ? false : tile.isSolid();
   }

   public void tick() {
      this.unprocessed = this.unprocessed + this.width * this.height * this.depth;
      int ticks = this.unprocessed / 400;
      this.unprocessed -= ticks * 400;

      for (int i = 0; i < ticks; i++) {
         int x = this.random.nextInt(this.width);
         int y = this.random.nextInt(this.depth);
         int z = this.random.nextInt(this.height);
         Tile tile = Tile.tiles[this.getTile(x, y, z)];
         if (tile != null) {
            tile.tick(this, x, y, z, this.random);
         }
      }
   }
}

==== com/mojang/rubydung/level/LevelListener.java ====
package com.mojang.rubydung.level;

public interface LevelListener {
   void tileChanged(int var1, int var2, int var3);

   void lightColumnChanged(int var1, int var2, int var3, int var4);

   void allChanged();
}

==== com/mojang/rubydung/level/LevelRenderer.java ====
package com.mojang.rubydung.level;

import com.mojang.rubydung.HitResult;
import com.mojang.rubydung.Player;
import com.mojang.rubydung.Textures;
import com.mojang.rubydung.level.tile.Tile;
import com.mojang.rubydung.phys.AABB;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import org.lwjgl.opengl.GL11;

public class LevelRenderer implements LevelListener {
   public static final int MAX_REBUILDS_PER_FRAME = 8;
   public static final int CHUNK_SIZE = 16;
   private Level level;
   private Chunk[] chunks;
   private int xChunks;
   private int yChunks;
   private int zChunks;

   public LevelRenderer(Level level) {
      this.level = level;
      level.addListener(this);
      this.xChunks = level.width / 16;
      this.yChunks = level.depth / 16;
      this.zChunks = level.height / 16;
      this.chunks = new Chunk[this.xChunks * this.yChunks * this.zChunks];

      for (int x = 0; x < this.xChunks; x++) {
         for (int y = 0; y < this.yChunks; y++) {
            for (int z = 0; z < this.zChunks; z++) {
               int x0 = x * 16;
               int y0 = y * 16;
               int z0 = z * 16;
               int x1 = (x + 1) * 16;
               int y1 = (y + 1) * 16;
               int z1 = (z + 1) * 16;
               if (x1 > level.width) {
                  x1 = level.width;
               }

               if (y1 > level.depth) {
                  y1 = level.depth;
               }

               if (z1 > level.height) {
                  z1 = level.height;
               }

               this.chunks[(x + y * this.xChunks) * this.zChunks + z] = new Chunk(level, x0, y0, z0, x1, y1, z1);
            }
         }
      }
   }

   public List<Chunk> getAllDirtyChunks() {
      ArrayList<Chunk> dirty = null;

      for (int i = 0; i < this.chunks.length; i++) {
         Chunk chunk = this.chunks[i];
         if (chunk.isDirty()) {
            if (dirty == null) {
               dirty = new ArrayList<>();
            }

            dirty.add(chunk);
         }
      }

      return dirty;
   }

   public void render(Player player, int layer) {
      GL11.glEnable(3553);
      int id = Textures.loadTexture("/terrain.png", 9728);
      GL11.glBindTexture(3553, id);
      Frustum frustum = Frustum.getFrustum();

      for (int i = 0; i < this.chunks.length; i++) {
         if (frustum.isVisible(this.chunks[i].aabb)) {
            this.chunks[i].render(layer);
         }
      }

      GL11.glDisable(3553);
   }

   public void updateDirtyChunks(Player player) {
      List<Chunk> dirty = this.getAllDirtyChunks();
      if (dirty != null) {
         Collections.sort(dirty, new DirtyChunkSorter(player, Frustum.getFrustum()));

         for (int i = 0; i < 8 && i < dirty.size(); i++) {
            dirty.get(i).rebuild();
         }
      }
   }

   public void pick(Player player) {
      Tesselator t = Tesselator.instance;
      float r = 3.0F;
      AABB box = player.bb.grow(r, r, r);
      int x0 = (int)box.x0;
      int x1 = (int)(box.x1 + 1.0F);
      int y0 = (int)box.y0;
      int y1 = (int)(box.y1 + 1.0F);
      int z0 = (int)box.z0;
      int z1 = (int)(box.z1 + 1.0F);
      GL11.glInitNames();

      for (int x = x0; x < x1; x++) {
         GL11.glPushName(x);

         for (int y = y0; y < y1; y++) {
            GL11.glPushName(y);

            for (int z = z0; z < z1; z++) {
               GL11.glPushName(z);
               Tile tile = Tile.tiles[this.level.getTile(x, y, z)];
               if (tile != null) {
                  GL11.glPushName(0);

                  for (int i = 0; i < 6; i++) {
                     GL11.glPushName(i);
                     t.init();
                     tile.renderFaceNoTexture(t, x, y, z, i);
                     t.flush();
                     GL11.glPopName();
                  }

                  GL11.glPopName();
               }

               GL11.glPopName();
            }

            GL11.glPopName();
         }

         GL11.glPopName();
      }
   }

   public void renderHit(HitResult h) {
      Tesselator t = Tesselator.instance;
      GL11.glEnable(3042);
      GL11.glBlendFunc(770, 1);
      GL11.glColor4f(1.0F, 1.0F, 1.0F, ((float)Math.sin(System.currentTimeMillis() / 100.0) * 0.2F + 0.4F) * 0.5F);
      t.init();
      Tile.rock.renderFaceNoTexture(t, h.x, h.y, h.z, h.f);
      t.flush();
      GL11.glDisable(3042);
   }

   public void setDirty(int x0, int y0, int z0, int x1, int y1, int z1) {
      x0 /= 16;
      x1 /= 16;
      y0 /= 16;
      y1 /= 16;
      z0 /= 16;
      z1 /= 16;
      if (x0 < 0) {
         x0 = 0;
      }

      if (y0 < 0) {
         y0 = 0;
      }

      if (z0 < 0) {
         z0 = 0;
      }

      if (x1 >= this.xChunks) {
         x1 = this.xChunks - 1;
      }

      if (y1 >= this.yChunks) {
         y1 = this.yChunks - 1;
      }

      if (z1 >= this.zChunks) {
         z1 = this.zChunks - 1;
      }

      for (int x = x0; x <= x1; x++) {
         for (int y = y0; y <= y1; y++) {
            for (int z = z0; z <= z1; z++) {
               this.chunks[(x + y * this.xChunks) * this.zChunks + z].setDirty();
            }
         }
      }
   }

   @Override
   public void tileChanged(int x, int y, int z) {
      this.setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
   }

   @Override
   public void lightColumnChanged(int x, int z, int y0, int y1) {
      this.setDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
   }

   @Override
   public void allChanged() {
      this.setDirty(0, 0, 0, this.level.width, this.level.depth, this.level.height);
   }
}

==== com/mojang/rubydung/level/PerlinNoiseFilter.java ====
package com.mojang.rubydung.level;

import java.util.Random;

public class PerlinNoiseFilter {
   Random random = new Random();
   int seed = this.random.nextInt();
   int levels = 0;
   int fuzz = 16;

   public PerlinNoiseFilter(int levels) {
      this.levels = levels;
   }

   public int[] read(int width, int height) {
      Random random = new Random();
      int[] tmp = new int[width * height];
      int level = this.levels;
      int step = width >> level;

      for (int y = 0; y < height; y += step) {
         for (int x = 0; x < width; x += step) {
            tmp[x + y * width] = (random.nextInt(256) - 128) * this.fuzz;
         }
      }

      for (int stepx = width >> level; stepx > 1; stepx /= 2) {
         int val = 256 * (stepx << level);
         int ss = stepx / 2;

         for (int y = 0; y < height; y += stepx) {
            for (int x = 0; x < width; x += stepx) {
               int ul = tmp[(x + 0) % width + (y + 0) % height * width];
               int ur = tmp[(x + stepx) % width + (y + 0) % height * width];
               int dl = tmp[(x + 0) % width + (y + stepx) % height * width];
               int dr = tmp[(x + stepx) % width + (y + stepx) % height * width];
               int m = (ul + dl + ur + dr) / 4 + random.nextInt(val * 2) - val;
               tmp[x + ss + (y + ss) * width] = m;
            }
         }

         for (int y = 0; y < height; y += stepx) {
            for (int x = 0; x < width; x += stepx) {
               int c = tmp[x + y * width];
               int r = tmp[(x + stepx) % width + y * width];
               int d = tmp[x + (y + stepx) % width * width];
               int mu = tmp[(x + ss & width - 1) + (y + ss - stepx & height - 1) * width];
               int ml = tmp[(x + ss - stepx & width - 1) + (y + ss & height - 1) * width];
               int m = tmp[(x + ss) % width + (y + ss) % height * width];
               int u = (c + r + m + mu) / 4 + random.nextInt(val * 2) - val;
               int l = (c + d + m + ml) / 4 + random.nextInt(val * 2) - val;
               tmp[x + ss + y * width] = u;
               tmp[x + (y + ss) * width] = l;
            }
         }
      }

      int[] result = new int[width * height];

      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
            result[x + y * width] = tmp[x % width + y % height * width] / 512 + 128;
         }
      }

      return result;
   }
}

==== com/mojang/rubydung/level/Tesselator.java ====
package com.mojang.rubydung.level;

import java.nio.Buffer;
import java.nio.FloatBuffer;
import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;

public class Tesselator {
   private static final int MAX_MEMORY_USE = 4194304;
   private static final int MAX_FLOATS = 524288;
   private FloatBuffer buffer = BufferUtils.createFloatBuffer(524288);
   private float[] array = new float[524288];
   private int vertices = 0;
   private float u;
   private float v;
   private float r;
   private float g;
   private float b;
   private boolean hasColor = false;
   private boolean hasTexture = false;
   private int len = 3;
   private int p = 0;
   public static Tesselator instance = new Tesselator();

   private Tesselator() {
   }

   public void flush() {
      ((Buffer)this.buffer).clear();
      this.buffer.put(this.array, 0, this.p);
      ((Buffer)this.buffer).flip();
      if (this.hasTexture && this.hasColor) {
         GL11.glInterleavedArrays(10794, 0, this.buffer);
      } else if (this.hasTexture) {
         GL11.glInterleavedArrays(10791, 0, this.buffer);
      } else if (this.hasColor) {
         GL11.glInterleavedArrays(10788, 0, this.buffer);
      } else {
         GL11.glInterleavedArrays(10785, 0, this.buffer);
      }

      GL11.glEnableClientState(32884);
      if (this.hasTexture) {
         GL11.glEnableClientState(32888);
      }

      if (this.hasColor) {
         GL11.glEnableClientState(32886);
      }

      GL11.glDrawArrays(7, 0, this.vertices);
      GL11.glDisableClientState(32884);
      if (this.hasTexture) {
         GL11.glDisableClientState(32888);
      }

      if (this.hasColor) {
         GL11.glDisableClientState(32886);
      }

      this.clear();
   }

   private void clear() {
      this.vertices = 0;
      ((Buffer)this.buffer).clear();
      this.p = 0;
   }

   public void init() {
      this.clear();
      this.hasColor = false;
      this.hasTexture = false;
   }

   public void tex(float u, float v) {
      if (!this.hasTexture) {
         this.len += 2;
      }

      this.hasTexture = true;
      this.u = u;
      this.v = v;
   }

   public void color(float r, float g, float b) {
      if (!this.hasColor) {
         this.len += 3;
      }

      this.hasColor = true;
      this.r = r;
      this.g = g;
      this.b = b;
   }

   public void vertexUV(float x, float y, float z, float u, float v) {
      this.tex(u, v);
      this.vertex(x, y, z);
   }

   public void vertex(float x, float y, float z) {
      if (this.hasTexture) {
         this.array[this.p++] = this.u;
         this.array[this.p++] = this.v;
      }

      if (this.hasColor) {
         this.array[this.p++] = this.r;
         this.array[this.p++] = this.g;
         this.array[this.p++] = this.b;
      }

      this.array[this.p++] = x;
      this.array[this.p++] = y;
      this.array[this.p++] = z;
      this.vertices++;
      if (this.p >= 524288 - this.len) {
         this.flush();
      }
   }
}

==== com/mojang/rubydung/level/tile/DirtTile.java ====
package com.mojang.rubydung.level.tile;

public class DirtTile extends Tile {
   protected DirtTile(int id, int tex) {
      super(id, tex);
   }
}

==== com/mojang/rubydung/level/tile/GrassTile.java ====
package com.mojang.rubydung.level.tile;

import com.mojang.rubydung.level.Level;
import java.util.Random;

public class GrassTile extends Tile {
   protected GrassTile(int id) {
      super(id);
      this.tex = 3;
   }

   @Override
   protected int getTexture(int face) {
      if (face == 1) {
         return 0;
      } else {
         return face == 0 ? 2 : 3;
      }
   }

   @Override
   public void tick(Level level, int x, int y, int z, Random random) {
      if (!level.isLit(x, y, z)) {
         level.setTile(x, y, z, Tile.dirt.id);
      } else {
         for (int i = 0; i < 4; i++) {
            int xt = x + random.nextInt(3) - 1;
            int yt = y + random.nextInt(5) - 3;
            int zt = z + random.nextInt(3) - 1;
            if (level.getTile(xt, yt, zt) == Tile.dirt.id && level.isLit(xt, yt, zt)) {
               level.setTile(xt, yt, zt, Tile.grass.id);
            }
         }
      }
   }
}

==== com/mojang/rubydung/level/tile/Tile.java ====
package com.mojang.rubydung.level.tile;

import com.mojang.rubydung.level.Level;
import com.mojang.rubydung.level.Tesselator;
import com.mojang.rubydung.particle.Particle;
import com.mojang.rubydung.particle.ParticleEngine;
import com.mojang.rubydung.phys.AABB;
import java.util.Random;

public class Tile {
   public static final Tile[] tiles = new Tile[256];
   public static final Tile empty = null;
   public static final Tile rock = new Tile(1, 1);
   public static final Tile grass = new GrassTile(2);
   public static final Tile dirt = new DirtTile(3, 2);
   public static final Tile stoneBrick = new Tile(4, 16);
   public static final Tile wood = new Tile(5, 4);
   public int tex;
   public final int id;

   protected Tile(int id) {
      tiles[id] = this;
      this.id = id;
   }

   protected Tile(int id, int tex) {
      this(id);
      this.tex = tex;
   }

   public void render(Tesselator t, Level level, int layer, int x, int y, int z) {
      float c1 = 1.0F;
      float c2 = 0.8F;
      float c3 = 0.6F;
      if (this.shouldRenderFace(level, x, y - 1, z, layer)) {
         t.color(c1, c1, c1);
         this.renderFace(t, x, y, z, 0);
      }

      if (this.shouldRenderFace(level, x, y + 1, z, layer)) {
         t.color(c1, c1, c1);
         this.renderFace(t, x, y, z, 1);
      }

      if (this.shouldRenderFace(level, x, y, z - 1, layer)) {
         t.color(c2, c2, c2);
         this.renderFace(t, x, y, z, 2);
      }

      if (this.shouldRenderFace(level, x, y, z + 1, layer)) {
         t.color(c2, c2, c2);
         this.renderFace(t, x, y, z, 3);
      }

      if (this.shouldRenderFace(level, x - 1, y, z, layer)) {
         t.color(c3, c3, c3);
         this.renderFace(t, x, y, z, 4);
      }

      if (this.shouldRenderFace(level, x + 1, y, z, layer)) {
         t.color(c3, c3, c3);
         this.renderFace(t, x, y, z, 5);
      }
   }

   private boolean shouldRenderFace(Level level, int x, int y, int z, int layer) {
      return !level.isSolidTile(x, y, z) && level.isLit(x, y, z) ^ layer == 1;
   }

   protected int getTexture(int face) {
      return this.tex;
   }

   public void renderFace(Tesselator t, int x, int y, int z, int face) {
      int tex = this.getTexture(face);
      float u0 = tex % 16 / 16.0F;
      float u1 = u0 + 0.0624375F;
      float v0 = tex / 16 / 16.0F;
      float v1 = v0 + 0.0624375F;
      float x0 = x + 0.0F;
      float x1 = x + 1.0F;
      float y0 = y + 0.0F;
      float y1 = y + 1.0F;
      float z0 = z + 0.0F;
      float z1 = z + 1.0F;
      if (face == 0) {
         t.vertexUV(x0, y0, z1, u0, v1);
         t.vertexUV(x0, y0, z0, u0, v0);
         t.vertexUV(x1, y0, z0, u1, v0);
         t.vertexUV(x1, y0, z1, u1, v1);
      }

      if (face == 1) {
         t.vertexUV(x1, y1, z1, u1, v1);
         t.vertexUV(x1, y1, z0, u1, v0);
         t.vertexUV(x0, y1, z0, u0, v0);
         t.vertexUV(x0, y1, z1, u0, v1);
      }

      if (face == 2) {
         t.vertexUV(x0, y1, z0, u1, v0);
         t.vertexUV(x1, y1, z0, u0, v0);
         t.vertexUV(x1, y0, z0, u0, v1);
         t.vertexUV(x0, y0, z0, u1, v1);
      }

      if (face == 3) {
         t.vertexUV(x0, y1, z1, u0, v0);
         t.vertexUV(x0, y0, z1, u0, v1);
         t.vertexUV(x1, y0, z1, u1, v1);
         t.vertexUV(x1, y1, z1, u1, v0);
      }

      if (face == 4) {
         t.vertexUV(x0, y1, z1, u1, v0);
         t.vertexUV(x0, y1, z0, u0, v0);
         t.vertexUV(x0, y0, z0, u0, v1);
         t.vertexUV(x0, y0, z1, u1, v1);
      }

      if (face == 5) {
         t.vertexUV(x1, y0, z1, u0, v1);
         t.vertexUV(x1, y0, z0, u1, v1);
         t.vertexUV(x1, y1, z0, u1, v0);
         t.vertexUV(x1, y1, z1, u0, v0);
      }
   }

   public void renderFaceNoTexture(Tesselator t, int x, int y, int z, int face) {
      float x0 = x + 0.0F;
      float x1 = x + 1.0F;
      float y0 = y + 0.0F;
      float y1 = y + 1.0F;
      float z0 = z + 0.0F;
      float z1 = z + 1.0F;
      if (face == 0) {
         t.vertex(x0, y0, z1);
         t.vertex(x0, y0, z0);
         t.vertex(x1, y0, z0);
         t.vertex(x1, y0, z1);
      }

      if (face == 1) {
         t.vertex(x1, y1, z1);
         t.vertex(x1, y1, z0);
         t.vertex(x0, y1, z0);
         t.vertex(x0, y1, z1);
      }

      if (face == 2) {
         t.vertex(x0, y1, z0);
         t.vertex(x1, y1, z0);
         t.vertex(x1, y0, z0);
         t.vertex(x0, y0, z0);
      }

      if (face == 3) {
         t.vertex(x0, y1, z1);
         t.vertex(x0, y0, z1);
         t.vertex(x1, y0, z1);
         t.vertex(x1, y1, z1);
      }

      if (face == 4) {
         t.vertex(x0, y1, z1);
         t.vertex(x0, y1, z0);
         t.vertex(x0, y0, z0);
         t.vertex(x0, y0, z1);
      }

      if (face == 5) {
         t.vertex(x1, y0, z1);
         t.vertex(x1, y0, z0);
         t.vertex(x1, y1, z0);
         t.vertex(x1, y1, z1);
      }
   }

   public AABB getAABB(int x, int y, int z) {
      return new AABB(x, y, z, x + 1, y + 1, z + 1);
   }

   public boolean blocksLight() {
      return true;
   }

   public boolean isSolid() {
      return true;
   }

   public void tick(Level level, int x, int y, int z, Random random) {
   }

   public void destroy(Level level, int x, int y, int z, ParticleEngine particleEngine) {
      int SD = 4;

      for (int xx = 0; xx < SD; xx++) {
         for (int yy = 0; yy < SD; yy++) {
            for (int zz = 0; zz < SD; zz++) {
               float xp = x + (xx + 0.5F) / SD;
               float yp = y + (yy + 0.5F) / SD;
               float zp = z + (zz + 0.5F) / SD;
               particleEngine.add(new Particle(level, xp, yp, zp, xp - x - 0.5F, yp - y - 0.5F, zp - z - 0.5F, this.tex));
            }
         }
      }
   }
}

==== com/mojang/rubydung/particle/Particle.java ====
package com.mojang.rubydung.particle;

import com.mojang.rubydung.Entity;
import com.mojang.rubydung.level.Level;
import com.mojang.rubydung.level.Tesselator;

public class Particle extends Entity {
   private float xd;
   private float yd;
   private float zd;
   public int tex;
   private float uo;
   private float vo;

   public Particle(Level level, float x, float y, float z, float xa, float ya, float za, int tex) {
      super(level);
      this.tex = tex;
      this.setSize(0.2F, 0.2F);
      this.heightOffset = this.bbHeight / 2.0F;
      this.setPos(x, y, z);
      this.xd = xa + (float)(Math.random() * 2.0 - 1.0) * 0.4F;
      this.yd = ya + (float)(Math.random() * 2.0 - 1.0) * 0.4F;
      this.zd = za + (float)(Math.random() * 2.0 - 1.0) * 0.4F;
      float speed = (float)(Math.random() + Math.random() + 1.0) * 0.15F;
      float dd = (float)Math.sqrt(this.xd * this.xd + this.yd * this.yd + this.zd * this.zd);
      this.xd = this.xd / dd * speed * 0.7F;
      this.yd = this.yd / dd * speed;
      this.zd = this.zd / dd * speed * 0.7F;
      this.uo = (float)Math.random() * 3.0F;
      this.vo = (float)Math.random() * 3.0F;
   }

   @Override
   public void tick() {
      this.xo = this.x;
      this.yo = this.y;
      this.zo = this.z;
      if (Math.random() < 0.1) {
         this.remove();
      }

      this.yd = (float)(this.yd - 0.06);
      this.move(this.xd, this.yd, this.zd);
      this.xd *= 0.98F;
      this.yd *= 0.98F;
      this.zd *= 0.98F;
      if (this.onGround) {
         this.xd *= 0.7F;
         this.zd *= 0.7F;
      }
   }

   public void render(Tesselator t, float a, float xa, float ya, float za) {
      float u0 = (this.tex % 16 + this.uo / 4.0F) / 16.0F;
      float u1 = u0 + 0.015609375F;
      float v0 = (this.tex / 16 + this.vo / 4.0F) / 16.0F;
      float v1 = v0 + 0.015609375F;
      float r = 0.1F;
      float x = this.xo + (this.x - this.xo) * a;
      float y = this.yo + (this.y - this.yo) * a;
      float z = this.zo + (this.z - this.zo) * a;
      t.vertexUV(x - xa * r, y - ya * r, z - za * r, u0, v1);
      t.vertexUV(x - xa * r, y + ya * r, z - za * r, u0, v0);
      t.vertexUV(x + xa * r, y + ya * r, z + za * r, u1, v0);
      t.vertexUV(x + xa * r, y - ya * r, z + za * r, u1, v1);
   }
}

==== com/mojang/rubydung/particle/ParticleEngine.java ====
package com.mojang.rubydung.particle;

import com.mojang.rubydung.Player;
import com.mojang.rubydung.Textures;
import com.mojang.rubydung.level.Level;
import com.mojang.rubydung.level.Tesselator;
import java.util.ArrayList;
import java.util.List;
import org.lwjgl.opengl.GL11;

public class ParticleEngine {
   protected Level level;
   private List<Particle> particles = new ArrayList<>();

   public ParticleEngine(Level level) {
      this.level = level;
   }

   public void add(Particle p) {
      this.particles.add(p);
   }

   public void tick() {
      for (int i = 0; i < this.particles.size(); i++) {
         Particle p = this.particles.get(i);
         p.tick();
         if (p.removed) {
            this.particles.remove(i--);
         }
      }
   }

   public void render(Player player, float a, int layer) {
      GL11.glEnable(3553);
      int id = Textures.loadTexture("/terrain.png", 9728);
      GL11.glBindTexture(3553, id);
      float xa = -((float)Math.cos(player.yRot * Math.PI / 180.0));
      float za = -((float)Math.sin(player.yRot * Math.PI / 180.0));
      float ya = 1.0F;
      Tesselator t = Tesselator.instance;
      GL11.glColor4f(0.8F, 0.8F, 0.8F, 1.0F);
      t.init();

      for (int i = 0; i < this.particles.size(); i++) {
         Particle p = this.particles.get(i);
         if (p.isLit() ^ layer == 1) {
            p.render(t, a, xa, ya, za);
         }
      }

      t.flush();
      GL11.glDisable(3553);
   }
}

==== com/mojang/rubydung/phys/AABB.java ====
package com.mojang.rubydung.phys;

public class AABB {
   private float epsilon = 0.0F;
   public float x0;
   public float y0;
   public float z0;
   public float x1;
   public float y1;
   public float z1;

   public AABB(float x0, float y0, float z0, float x1, float y1, float z1) {
      this.x0 = x0;
      this.y0 = y0;
      this.z0 = z0;
      this.x1 = x1;
      this.y1 = y1;
      this.z1 = z1;
   }

   public AABB expand(float xa, float ya, float za) {
      float _x0 = this.x0;
      float _y0 = this.y0;
      float _z0 = this.z0;
      float _x1 = this.x1;
      float _y1 = this.y1;
      float _z1 = this.z1;
      if (xa < 0.0F) {
         _x0 += xa;
      }

      if (xa > 0.0F) {
         _x1 += xa;
      }

      if (ya < 0.0F) {
         _y0 += ya;
      }

      if (ya > 0.0F) {
         _y1 += ya;
      }

      if (za < 0.0F) {
         _z0 += za;
      }

      if (za > 0.0F) {
         _z1 += za;
      }

      return new AABB(_x0, _y0, _z0, _x1, _y1, _z1);
   }

   public AABB grow(float xa, float ya, float za) {
      float _x0 = this.x0 - xa;
      float _y0 = this.y0 - ya;
      float _z0 = this.z0 - za;
      float _x1 = this.x1 + xa;
      float _y1 = this.y1 + ya;
      float _z1 = this.z1 + za;
      return new AABB(_x0, _y0, _z0, _x1, _y1, _z1);
   }

   public float clipXCollide(AABB c, float xa) {
      if (c.y1 <= this.y0 || c.y0 >= this.y1) {
         return xa;
      } else if (!(c.z1 <= this.z0) && !(c.z0 >= this.z1)) {
         if (xa > 0.0F && c.x1 <= this.x0) {
            float max = this.x0 - c.x1 - this.epsilon;
            if (max < xa) {
               xa = max;
            }
         }

         if (xa < 0.0F && c.x0 >= this.x1) {
            float max = this.x1 - c.x0 + this.epsilon;
            if (max > xa) {
               xa = max;
            }
         }

         return xa;
      } else {
         return xa;
      }
   }

   public float clipYCollide(AABB c, float ya) {
      if (c.x1 <= this.x0 || c.x0 >= this.x1) {
         return ya;
      } else if (!(c.z1 <= this.z0) && !(c.z0 >= this.z1)) {
         if (ya > 0.0F && c.y1 <= this.y0) {
            float max = this.y0 - c.y1 - this.epsilon;
            if (max < ya) {
               ya = max;
            }
         }

         if (ya < 0.0F && c.y0 >= this.y1) {
            float max = this.y1 - c.y0 + this.epsilon;
            if (max > ya) {
               ya = max;
            }
         }

         return ya;
      } else {
         return ya;
      }
   }

   public float clipZCollide(AABB c, float za) {
      if (c.x1 <= this.x0 || c.x0 >= this.x1) {
         return za;
      } else if (!(c.y1 <= this.y0) && !(c.y0 >= this.y1)) {
         if (za > 0.0F && c.z1 <= this.z0) {
            float max = this.z0 - c.z1 - this.epsilon;
            if (max < za) {
               za = max;
            }
         }

         if (za < 0.0F && c.z0 >= this.z1) {
            float max = this.z1 - c.z0 + this.epsilon;
            if (max > za) {
               za = max;
            }
         }

         return za;
      } else {
         return za;
      }
   }

   public boolean intersects(AABB c) {
      if (c.x1 <= this.x0 || c.x0 >= this.x1) {
         return false;
      } else {
         return c.y1 <= this.y0 || c.y0 >= this.y1 ? false : !(c.z1 <= this.z0) && !(c.z0 >= this.z1);
      }
   }

   public void move(float xa, float ya, float za) {
      this.x0 += xa;
      this.y0 += ya;
      this.z0 += za;
      this.x1 += xa;
      this.y1 += ya;
      this.z1 += za;
   }
}

==== com/mojang/rubydung/Player.java ====
package com.mojang.rubydung;

import com.mojang.rubydung.level.Level;
import org.lwjgl.input.Keyboard;

public class Player extends Entity {
   public Player(Level level) {
      super(level);
      this.heightOffset = 1.62F;
   }

   @Override
   public void tick() {
      this.xo = this.x;
      this.yo = this.y;
      this.zo = this.z;
      float xa = 0.0F;
      float ya = 0.0F;
      if (Keyboard.isKeyDown(19)) {
         this.resetPos();
      }

      if (Keyboard.isKeyDown(200) || Keyboard.isKeyDown(17)) {
         ya--;
      }

      if (Keyboard.isKeyDown(208) || Keyboard.isKeyDown(31)) {
         ya++;
      }

      if (Keyboard.isKeyDown(203) || Keyboard.isKeyDown(30)) {
         xa--;
      }

      if (Keyboard.isKeyDown(205) || Keyboard.isKeyDown(32)) {
         xa++;
      }

      if ((Keyboard.isKeyDown(57) || Keyboard.isKeyDown(219)) && this.onGround) {
         this.yd = 0.5F;
      }

      this.moveRelative(xa, ya, this.onGround ? 0.1F : 0.02F);
      this.yd = (float)(this.yd - 0.08);
      this.move(this.xd, this.yd, this.zd);
      this.xd *= 0.91F;
      this.yd *= 0.98F;
      this.zd *= 0.91F;
      if (this.onGround) {
         this.xd *= 0.7F;
         this.zd *= 0.7F;
      }
   }
}

==== com/mojang/rubydung/RubyDung.java ====
package com.mojang.rubydung;

import com.mojang.rubydung.character.Zombie;
import com.mojang.rubydung.level.Chunk;
import com.mojang.rubydung.level.Frustum;
import com.mojang.rubydung.level.Level;
import com.mojang.rubydung.level.LevelRenderer;
import com.mojang.rubydung.level.Tesselator;
import com.mojang.rubydung.level.tile.Tile;
import com.mojang.rubydung.particle.ParticleEngine;
import java.io.IOException;
import java.nio.Buffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import javax.swing.JOptionPane;
import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;

public class RubyDung implements Runnable {
   private static final boolean FULLSCREEN_MODE = true;
   private int width;
   private int height;
   private FloatBuffer fogColor0 = BufferUtils.createFloatBuffer(4);
   private FloatBuffer fogColor1 = BufferUtils.createFloatBuffer(4);
   private Timer timer = new Timer(20.0F);
   private Level level;
   private LevelRenderer levelRenderer;
   private Player player;
   private int paintTexture = 1;
   private ParticleEngine particleEngine;
   private ArrayList<Zombie> zombies = new ArrayList<>();
   private IntBuffer viewportBuffer = BufferUtils.createIntBuffer(16);
   private IntBuffer selectBuffer = BufferUtils.createIntBuffer(2000);
   private HitResult hitResult = null;
   FloatBuffer lb = BufferUtils.createFloatBuffer(16);

   public void init() throws LWJGLException, IOException {
      int col0 = 16710650;
      int col1 = 920330;
      float fr = 0.5F;
      float fg = 0.8F;
      float fb = 1.0F;
      this.fogColor0.put(new float[]{(col0 >> 16 & 0xFF) / 255.0F, (col0 >> 8 & 0xFF) / 255.0F, (col0 & 0xFF) / 255.0F, 1.0F});
      ((Buffer)this.fogColor0).flip();
      this.fogColor1.put(new float[]{(col1 >> 16 & 0xFF) / 255.0F, (col1 >> 8 & 0xFF) / 255.0F, (col1 & 0xFF) / 255.0F, 1.0F});
      ((Buffer)this.fogColor1).flip();
      Display.setFullscreen(true);
      Display.create();
      Keyboard.create();
      Mouse.create();
      this.width = Display.getDisplayMode().getWidth();
      this.height = Display.getDisplayMode().getHeight();
      GL11.glEnable(3553);
      GL11.glShadeModel(7425);
      GL11.glClearColor(fr, fg, fb, 0.0F);
      GL11.glClearDepth(1.0);
      GL11.glEnable(2929);
      GL11.glDepthFunc(515);
      GL11.glMatrixMode(5889);
      GL11.glLoadIdentity();
      GL11.glMatrixMode(5888);
      this.level = new Level(256, 256, 64);
      this.levelRenderer = new LevelRenderer(this.level);
      this.player = new Player(this.level);
      this.particleEngine = new ParticleEngine(this.level);
      Mouse.setGrabbed(true);

      for (int i = 0; i < 10; i++) {
         Zombie zombie = new Zombie(this.level, 128.0F, 0.0F, 128.0F);
         zombie.resetPos();
         this.zombies.add(zombie);
      }
   }

   public void destroy() {
      this.level.save();
      Mouse.destroy();
      Keyboard.destroy();
      Display.destroy();
   }

   @Override
   public void run() {
      try {
         this.init();
      } catch (Exception var9) {
         JOptionPane.showMessageDialog(null, var9.toString(), "Failed to start RubyDung", 0);
         System.exit(0);
      }

      long lastTime = System.currentTimeMillis();
      int frames = 0;

      try {
         while (!Keyboard.isKeyDown(1) && !Display.isCloseRequested()) {
            this.timer.advanceTime();

            for (int i = 0; i < this.timer.ticks; i++) {
               this.tick();
            }

            this.render(this.timer.a);
            frames++;

            while (System.currentTimeMillis() >= lastTime + 1000L) {
               System.out.println(frames + " fps, " + Chunk.updates);
               Chunk.updates = 0;
               lastTime += 1000L;
               frames = 0;
            }
         }
      } catch (Exception var10) {
         var10.printStackTrace();
      } finally {
         this.destroy();
      }
   }

   public void tick() {
      while (Keyboard.next()) {
         if (Keyboard.getEventKeyState()) {
            if (Keyboard.getEventKey() == 28) {
               this.level.save();
            }

            if (Keyboard.getEventKey() == 2) {
               this.paintTexture = 1;
            }

            if (Keyboard.getEventKey() == 3) {
               this.paintTexture = 3;
            }

            if (Keyboard.getEventKey() == 4) {
               this.paintTexture = 4;
            }

            if (Keyboard.getEventKey() == 5) {
               this.paintTexture = 5;
            }

            if (Keyboard.getEventKey() == 34) {
               this.zombies.add(new Zombie(this.level, this.player.x, this.player.y, this.player.z));
            }
         }
      }

      this.level.tick();
      this.particleEngine.tick();

      for (int i = 0; i < this.zombies.size(); i++) {
         this.zombies.get(i).tick();
         if (this.zombies.get(i).removed) {
            this.zombies.remove(i--);
         }
      }

      this.player.tick();
   }

   private void moveCameraToPlayer(float a) {
      GL11.glTranslatef(0.0F, 0.0F, -0.3F);
      GL11.glRotatef(this.player.xRot, 1.0F, 0.0F, 0.0F);
      GL11.glRotatef(this.player.yRot, 0.0F, 1.0F, 0.0F);
      float x = this.player.xo + (this.player.x - this.player.xo) * a;
      float y = this.player.yo + (this.player.y - this.player.yo) * a;
      float z = this.player.zo + (this.player.z - this.player.zo) * a;
      GL11.glTranslatef(-x, -y, -z);
   }

   private void setupCamera(float a) {
      GL11.glMatrixMode(5889);
      GL11.glLoadIdentity();
      GLU.gluPerspective(70.0F, (float)this.width / this.height, 0.05F, 1000.0F);
      GL11.glMatrixMode(5888);
      GL11.glLoadIdentity();
      this.moveCameraToPlayer(a);
   }

   private void setupOrthoCamera() {
      GL11.glMatrixMode(5889);
      GL11.glLoadIdentity();
      GL11.glOrtho(0.0, this.width, this.height, 0.0, 100.0, 300.0);
      GL11.glMatrixMode(5888);
      GL11.glLoadIdentity();
      GL11.glTranslatef(0.0F, 0.0F, -200.0F);
   }

   private void setupPickCamera(float a, int x, int y) {
      GL11.glMatrixMode(5889);
      GL11.glLoadIdentity();
      ((Buffer)this.viewportBuffer).clear();
      GL11.glGetInteger(2978, this.viewportBuffer);
      ((Buffer)this.viewportBuffer).flip();
      ((Buffer)this.viewportBuffer).limit(16);
      GLU.gluPickMatrix(x, y, 5.0F, 5.0F, this.viewportBuffer);
      GLU.gluPerspective(70.0F, (float)this.width / this.height, 0.05F, 1000.0F);
      GL11.glMatrixMode(5888);
      GL11.glLoadIdentity();
      this.moveCameraToPlayer(a);
   }

   private void pick(float a) {
      ((Buffer)this.selectBuffer).clear();
      GL11.glSelectBuffer(this.selectBuffer);
      GL11.glRenderMode(7170);
      this.setupPickCamera(a, this.width / 2, this.height / 2);
      this.levelRenderer.pick(this.player);
      int hits = GL11.glRenderMode(7168);
      ((Buffer)this.selectBuffer).flip();
      ((Buffer)this.selectBuffer).limit(this.selectBuffer.capacity());
      long closest = 0L;
      int[] names = new int[10];
      int hitNameCount = 0;

      for (int i = 0; i < hits; i++) {
         int nameCount = this.selectBuffer.get();
         long minZ = this.selectBuffer.get();
         this.selectBuffer.get();
         if (minZ >= closest && i != 0) {
            for (int j = 0; j < nameCount; j++) {
               this.selectBuffer.get();
            }
         } else {
            closest = minZ;
            hitNameCount = nameCount;

            for (int j = 0; j < nameCount; j++) {
               names[j] = this.selectBuffer.get();
            }
         }
      }

      if (hitNameCount > 0) {
         this.hitResult = new HitResult(names[0], names[1], names[2], names[3], names[4]);
      } else {
         this.hitResult = null;
      }
   }

   public void render(float a) {
      float xo = Mouse.getDX();
      float yo = Mouse.getDY();
      this.player.turn(xo, yo);
      this.pick(a);

      while (Mouse.next()) {
         if (Mouse.getEventButton() == 1 && Mouse.getEventButtonState() && this.hitResult != null) {
            Tile oldTile = Tile.tiles[this.level.getTile(this.hitResult.x, this.hitResult.y, this.hitResult.z)];
            boolean changed = this.level.setTile(this.hitResult.x, this.hitResult.y, this.hitResult.z, 0);
            if (oldTile != null && changed) {
               oldTile.destroy(this.level, this.hitResult.x, this.hitResult.y, this.hitResult.z, this.particleEngine);
            }
         }

         if (Mouse.getEventButton() == 0 && Mouse.getEventButtonState() && this.hitResult != null) {
            int x = this.hitResult.x;
            int y = this.hitResult.y;
            int z = this.hitResult.z;
            if (this.hitResult.f == 0) {
               y--;
            }

            if (this.hitResult.f == 1) {
               y++;
            }

            if (this.hitResult.f == 2) {
               z--;
            }

            if (this.hitResult.f == 3) {
               z++;
            }

            if (this.hitResult.f == 4) {
               x--;
            }

            if (this.hitResult.f == 5) {
               x++;
            }

            this.level.setTile(x, y, z, this.paintTexture);
         }
      }

      GL11.glClear(16640);
      this.setupCamera(a);
      GL11.glEnable(2884);
      Frustum frustum = Frustum.getFrustum();
      this.levelRenderer.updateDirtyChunks(this.player);
      this.setupFog(0);
      GL11.glEnable(2912);
      this.levelRenderer.render(this.player, 0);

      for (int i = 0; i < this.zombies.size(); i++) {
         Zombie zombie = this.zombies.get(i);
         if (zombie.isLit() && frustum.isVisible(zombie.bb)) {
            this.zombies.get(i).render(a);
         }
      }

      this.particleEngine.render(this.player, a, 0);
      this.setupFog(1);
      this.levelRenderer.render(this.player, 1);

      for (int ix = 0; ix < this.zombies.size(); ix++) {
         Zombie zombie = this.zombies.get(ix);
         if (!zombie.isLit() && frustum.isVisible(zombie.bb)) {
            this.zombies.get(ix).render(a);
         }
      }

      this.particleEngine.render(this.player, a, 1);
      GL11.glDisable(2896);
      GL11.glDisable(3553);
      GL11.glDisable(2912);
      if (this.hitResult != null) {
         this.levelRenderer.renderHit(this.hitResult);
      }

      this.drawGui(a);
      Display.update();
   }

   private void drawGui(float a) {
      GL11.glClear(256);
      this.setupOrthoCamera();
      GL11.glPushMatrix();
      GL11.glTranslatef(this.width - 48, 48.0F, 0.0F);
      Tesselator t = Tesselator.instance;
      GL11.glScalef(48.0F, 48.0F, 48.0F);
      GL11.glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
      GL11.glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
      GL11.glTranslatef(1.5F, -0.5F, -0.5F);
      int id = Textures.loadTexture("/terrain.png", 9728);
      GL11.glBindTexture(3553, id);
      GL11.glEnable(3553);
      t.init();
      Tile.tiles[this.paintTexture].render(t, this.level, 0, -2, 0, 0);
      t.flush();
      GL11.glDisable(3553);
      GL11.glPopMatrix();
      int wc = this.width / 2;
      int hc = this.height / 2;
      GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
      t.init();
      t.vertex(wc + 1, hc - 8, 0.0F);
      t.vertex(wc - 0, hc - 8, 0.0F);
      t.vertex(wc - 0, hc + 9, 0.0F);
      t.vertex(wc + 1, hc + 9, 0.0F);
      t.vertex(wc + 9, hc - 0, 0.0F);
      t.vertex(wc - 8, hc - 0, 0.0F);
      t.vertex(wc - 8, hc + 1, 0.0F);
      t.vertex(wc + 9, hc + 1, 0.0F);
      t.flush();
   }

   private void setupFog(int i) {
      if (i == 0) {
         GL11.glFogi(2917, 2048);
         GL11.glFogf(2914, 0.001F);
         GL11.glFog(2918, this.fogColor0);
         GL11.glDisable(2896);
      } else if (i == 1) {
         GL11.glFogi(2917, 2048);
         GL11.glFogf(2914, 0.06F);
         GL11.glFog(2918, this.fogColor1);
         GL11.glEnable(2896);
         GL11.glEnable(2903);
         float br = 0.6F;
         GL11.glLightModel(2899, this.getBuffer(br, br, br, 1.0F));
      }
   }

   private FloatBuffer getBuffer(float a, float b, float c, float d) {
      ((Buffer)this.lb).clear();
      this.lb.put(a).put(b).put(c).put(d);
      ((Buffer)this.lb).flip();
      return this.lb;
   }

   public static void checkError() {
      int e = GL11.glGetError();
      if (e != 0) {
         throw new IllegalStateException(GLU.gluErrorString(e));
      }
   }

   public static void main(String[] args) throws LWJGLException {
      new Thread(new RubyDung()).start();
   }
}

==== com/mojang/rubydung/Textures.java ====
package com.mojang.rubydung;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;
import javax.imageio.ImageIO;
import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.glu.GLU;

public class Textures {
   private static HashMap<String, Integer> idMap = new HashMap<>();

   public static int loadTexture(String resourceName, int mode) {
      try {
         if (idMap.containsKey(resourceName)) {
            return idMap.get(resourceName);
         } else {
            IntBuffer ib = BufferUtils.createIntBuffer(1);
            ((Buffer)ib).clear();
            GL11.glGenTextures(ib);
            int id = ib.get(0);
            idMap.put(resourceName, id);
            System.out.println(resourceName + " -> " + id);
            GL11.glBindTexture(3553, id);
            GL11.glTexParameteri(3553, 10241, mode);
            GL11.glTexParameteri(3553, 10240, mode);
            BufferedImage img = ImageIO.read(Textures.class.getResourceAsStream(resourceName));
            int w = img.getWidth();
            int h = img.getHeight();
            ByteBuffer pixels = BufferUtils.createByteBuffer(w * h * 4);
            int[] rawPixels = new int[w * h];
            img.getRGB(0, 0, w, h, rawPixels, 0, w);

            for (int i = 0; i < rawPixels.length; i++) {
               int a = rawPixels[i] >> 24 & 0xFF;
               int r = rawPixels[i] >> 16 & 0xFF;
               int g = rawPixels[i] >> 8 & 0xFF;
               int b = rawPixels[i] & 0xFF;
               rawPixels[i] = a << 24 | b << 16 | g << 8 | r;
            }

            pixels.asIntBuffer().put(rawPixels);
            GLU.gluBuild2DMipmaps(3553, 6408, w, h, 6408, 5121, pixels);
            return id;
         }
      } catch (IOException var14) {
         throw new RuntimeException("!!");
      }
   }
}

==== com/mojang/rubydung/Timer.java ====
package com.mojang.rubydung;

public class Timer {
   private static final long NS_PER_SECOND = 1000000000L;
   private static final long MAX_NS_PER_UPDATE = 1000000000L;
   private static final int MAX_TICKS_PER_UPDATE = 100;
   private float ticksPerSecond;
   private long lastTime;
   public int ticks;
   public float a;
   public float timeScale = 1.0F;
   public float fps = 0.0F;
   public float passedTime = 0.0F;

   public Timer(float ticksPerSecond) {
      this.ticksPerSecond = ticksPerSecond;
      this.lastTime = System.nanoTime();
   }

   public void advanceTime() {
      long now = System.nanoTime();
      long passedNs = now - this.lastTime;
      this.lastTime = now;
      if (passedNs < 0L) {
         passedNs = 0L;
      }

      if (passedNs > 1000000000L) {
         passedNs = 1000000000L;
      }

      this.fps = (float)(1000000000L / passedNs);
      this.passedTime = this.passedTime + (float)passedNs * this.timeScale * this.ticksPerSecond / 1.0E9F;
      this.ticks = (int)this.passedTime;
      if (this.ticks > 100) {
         this.ticks = 100;
      }

      this.passedTime = this.passedTime - this.ticks;
      this.a = this.passedTime;
   }
}

