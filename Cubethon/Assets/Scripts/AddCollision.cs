using UnityEngine;

public class AddCollision : MonoBehaviour {
	public PlayerMovement movement;

	void OnCollisionEnter(Collision collisionInfo)
	{
		if (collisionInfo.collider.tag == "Obstacle") {
			movement.enabled = false;
			FindObjectOfType<GameManager>().EndGame();
		}
	}

}
